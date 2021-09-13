#version 330 core

layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform vec3 viewPosition;

uniform mat4 viewMatrix;

uniform vec3 lightColor = {10.0, 10.0, 10.0};

uniform vec3 pixelOffset;

uniform sampler2D gBufferPositionMetallic; // Contains both the position and metallic values
uniform sampler2D gBufferNormalRoughness; // Contains both the normal and roughness values
uniform sampler2D gBufferAlbedoAmbientOcclusion; // Contains both the albedo and ambient occlusion values

uniform sampler2D shadowMap;
uniform float far;
uniform sampler2D shadowMap2;
uniform float far2;
uniform sampler2D shadowMap3;

uniform mat4 lightViewProjectionMatrix;
uniform mat4 lightViewProjectionMatrix2;
uniform mat4 lightViewProjectionMatrix3;

uniform samplerCube irradianceCubeMap;
uniform samplerCube prefilterCubeMap;
uniform sampler2D brdfLUT;

uniform bool debug;

const float PI = 3.1415927;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

/* Approximation of fresnel schlick 
https://seblagarde.wordpress.com/2012/06/03/spherical-gaussien-approximation-for-blinn-phong-phong-and-fresnel/
difference is unnoticable and slightly cheaper
*/
vec3 fresnelSphericalGaussian(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(2, -5.55473 * cosTheta - 6.98316 * cosTheta);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float calculateShadowFactor(vec4 position, vec3 normal, mat4 lightViewProjectionMatrix, sampler2D shadowMap_) {
    
    // Moving the current fragment to light space
    vec4 fragmentLightSpace = lightViewProjectionMatrix * position + vec4(pixelOffset,0.0);

    // Obtaining the coordinates to extract the depth from the shadow map 
    // and moving them to the range of [0,1] which is the range of the depth in the shadow map and the texture coordinates
    // to sample from the shadow map
	vec3 shadowMapCoords = fragmentLightSpace.xyz * 0.5 + 0.5;

	// Extracting the closest depth from the light's point of view
	//float shadowMapDepth = texture(shadowMap, shadowMapCoords.xy).r;   

	// Extracting the depth of the current fragment from the light's point of view
	float currentFragmentDepth = shadowMapCoords.z; 

    if(currentFragmentDepth > 1.0)
        return 0.0f;
    
	float bias = max(0.05 * (1.0 - dot(normalize(normal),  -1 *normalize(vec3(-1.0, -1.0, 0.0)))), 0.005);

	// PCF Filter
    float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap_, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{

			float shadowMapDepth = texture(shadowMap_, shadowMapCoords.xy + vec2(x, y) * texelSize).r; 

            // if the current fragment is closer than the fragment renderered from the lights perspective
            // then the object is not in shadow, otherwise it is
			shadow += currentFragmentDepth - bias > shadowMapDepth ? 1.0 : 0.0;        
		}    
	}
    shadow/=9.0;

    /*float shadowMapDepth = texture(shadowMap_, shadowMapCoords.xy).r; 

    // if the current fragment is closer than the fragment renderered from the lights perspective
    // then the object is not in shadow, otherwise it is
    float shadow = currentFragmentDepth - bias > shadowMapDepth ? 1.0 : 0.0; */
    

	return shadow;
}

float calculateShadows(vec4 position, vec3 normal) {
    
    // The near and far planes distance for each of the cascades is relative to the position of the camera
    // so to check whether a fragment is within a certain cascade we need to move its position from world space
    // to view space
    vec4 positionViewSpace = viewMatrix * position;
    float fragmentDepthViewSpace = -positionViewSpace.z;

    if (fragmentDepthViewSpace < far) {
   
        return calculateShadowFactor(position, normal, lightViewProjectionMatrix, shadowMap);

    } else if (fragmentDepthViewSpace < far2) {

        return calculateShadowFactor(position, normal, lightViewProjectionMatrix2, shadowMap2);

    } else {

        return calculateShadowFactor(position, normal, lightViewProjectionMatrix3, shadowMap3);
    }
    
}

vec3 LambertianDiffuse(vec3 diffuseColor) {
    return diffuseColor / PI;
}

vec3 OrenNayarDiffuse(vec3 diffuseColor, float roughness, vec3 lightDir, vec3 viewDir) {
    
    vec3 lambert = LambertianDiffuse(diffuseColor);

    if (roughness == 0.0) 
        return lambert;

    // Moving to spherical coordinates assuming radius 1
    float thetaI = acos(lightDir.z);
    float phiI = atan(lightDir.y / lightDir.x);

    float thetaR = acos(viewDir.z);
    float phiR = atan(viewDir.y / viewDir.x);

    float roughnessSqrd = roughness * roughness;
    float A = 1.0 - 0.5 * (roughnessSqrd / (roughnessSqrd + 0.33));
    float B = 0.45 * (roughnessSqrd / (roughnessSqrd + 0.09));
    float alpha = max(thetaI, thetaR);
    float beta = min(thetaI, thetaR);

    return lambert * (A + (B*max(0.0, cos(phiI - phiR))) * sin(alpha) * tan(beta));

}

vec3 pbr(vec3 position, vec3 normal, vec3 albedo, float metallic, float roughness, float ambientOcclusion) {
    
    vec3 V = normalize(viewPosition - position);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    // calculate per-light radiance
    vec3 L = -1 * normalize(vec3(-1.0, -1.0, 0.0));
    vec3 H = normalize(V + L);
    vec3 radiance = lightColor;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, H, roughness);   
    float G   = GeometrySmith(normal, V, L, roughness);      
    vec3 F    = fresnelSphericalGaussian(max(dot(H, V), 0.0), F0);
           
    vec3 numerator    = NDF * G * F; 
    float denominator = 4 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
    vec3 specular = numerator / denominator;
    vec3 diffuse = 
    /*OrenNayarDiffuse(albedo, roughness, L, V);*/ LambertianDiffuse(albedo);
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(normal, L), 0.0);     
    
    float shadow = calculateShadows(vec4(position,1.0),normal);

    // add to outgoing radiance Lo
    Lo += (kD * diffuse + specular) * radiance * NdotL * (1-shadow);  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 kS2 = fresnelSchlick(max(dot(normal, V), 0.0), F0);
    vec3 kD2 = 1.0 - kS2;
    kD *= 1.0 - metallic;	  
    vec3 irradiance = texture(irradianceCubeMap, normal).rgb;
    vec3 diffuse2 = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 R = reflect(-V, normal); 
    vec3 prefilteredColor = textureLod(prefilterCubeMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(normal, V), 0.0), roughness)).rg;
    vec3 specular2 = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD2 * diffuse2 + specular2) * ambientOcclusion;

    vec3 color = ambient + Lo;

    return color;
}

void main(void)
{

    // 1. Extract position, metallic map, normal, roughness map, albedo and ambient occlusion map values
    // from each of the buffers that compose the gBuffer
    vec4 positionMetallic = texture(gBufferPositionMetallic, fragTextCoords);
    vec4 normalRoughness = texture(gBufferNormalRoughness, fragTextCoords);
    vec4 albedoAmbientOcclusion = texture(gBufferAlbedoAmbientOcclusion, fragTextCoords);
    
    vec3 position = positionMetallic.rgb; 
    float metallic = positionMetallic.a;
    vec3 normal = normalRoughness.rgb; 
    float roughness = normalRoughness.a;
    vec3 albedo = albedoAmbientOcclusion.rgb; 
    float ambientOcclusion = albedoAmbientOcclusion.a;

    // 2. Calculate color using PBR
    vec3 color = pbr(position, normal, albedo, metallic, roughness, ambientOcclusion);

    vec4 positionViewSpace = viewMatrix * vec4(position,1.0);
    float fragmentDepthViewSpace = -positionViewSpace.z;

    // HDR tonemapping (Reinhard)
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    if (debug) {
        if (fragmentDepthViewSpace < far) {
            color.r += 0.1;
        } else if (fragmentDepthViewSpace < far2) {
            color.g += 0.1;
        } else {
            color.b += 0.1;
        }
    }

    fragmentColor = vec4(color, 1.0);
}
