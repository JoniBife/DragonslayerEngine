#version 330 core

layout (location = 0) out vec4 fragmentColor;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float radiance;
};

const float CUTOFF_THRESHOLD = 0.001; // Ignore point light contributions below this value
struct PointLight {
    vec3 position;
    float radiance;
    vec3 color;
    float radius;
};

in vec2 fragTextCoords;

// Camera inputs
uniform vec3 viewPosition;
uniform mat4 viewMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 inverseProjectionMatrix;
uniform float projectionNear;
uniform float projectionFar;

// Light inputs
uniform DirectionalLight directionalLights[4];
uniform uint numberOfDirectionalLights; // This solution works with structs but not with samplers on open gl version 3.30

// Using the std140 to have a consistent memory layout over any graphics card
#define MAX_POINT_LIGHTS 2000
layout (std140) uniform pointLightsBlock {
    PointLight pointLights[MAX_POINT_LIGHTS];
};
uniform uint numberOfPointLights;
//uniform float intensity;
//uniform float radius;
//uniform vec3 lposition;

// G-Buffer inputs ----------------------------------------------------------
uniform sampler2D gBufferMetallic; // Contains the metallic values 
uniform sampler2D gBufferNormalRoughness; // Contains both the normal and roughness values
uniform sampler2D gBufferAlbedoAmbientOcclusion; // Contains both the albedo and ambient occlusion values
uniform sampler2D gBufferDepth;

// IBL inputs
uniform samplerCube irradianceCubeMap;
uniform samplerCube prefilterCubeMap;
uniform sampler2D brdfLUT;

// Cascaded shadow mapping inputs --------------------------------------------
uniform vec3 pixelOffset;

#define MAX_CASCADES 6
uniform sampler2D shadowMaps[MAX_CASCADES];
uniform float cascadesPlanes[MAX_CASCADES + 1]; // +1 because we are counting the first near plane
uniform mat4 lightViewProjectionMatrices[MAX_CASCADES];
uniform uint numberOfCascades;

// Debug inputs
uniform bool debug;

const float PI = 3.1415927;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
} 

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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

    for (uint i = 0u; i < numberOfCascades; ++i) {
        if (fragmentDepthViewSpace > cascadesPlanes[i] && fragmentDepthViewSpace < cascadesPlanes[i + 1u]) {
            return calculateShadowFactor(position, normal, lightViewProjectionMatrices[i], shadowMaps[i]);
        }
    }

    return 0;
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

    //  Directional lights contribution
    for (uint i = 0u; i < numberOfDirectionalLights; ++i) {
        
        DirectionalLight directionalLight = directionalLights[i];
      
        // calculate per-light radiance
        vec3 L = -1 * directionalLight.direction;
        vec3 H = normalize(V + L);
        vec3 radiance = directionalLight.color * directionalLight.radiance;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, roughness);   
        float G   = GeometrySmith(normal, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
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
    }

    // Point lights contribution
    for (uint i = 0u; i < numberOfPointLights; ++i) {
        
        PointLight pointLight = pointLights[i];

        //pointLight.position = lposition;
        //pointLight.radius = radius;
        //pointLight.color = vec3(1.0, 0.0, 0.0);
        //pointLight.radiance = intensity;

        // calculate per-light radiance
        vec3 L = -1 * (position - pointLight.position);
        float distanceToLight = length(L);

        if (distanceToLight <= pointLight.radius) {

            vec3 H = normalize(V + L);

            // Point Light attenuation without constant, linear and quadratic terms (a much easier to use solution)
            // https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
            /*float denum = distanceToLight/pointLight.radius + 1;
            float attenuation = 1.0 / (denum * denum);
            // Biasing 
            attenuation = attenuation - CUTOFF_THRESHOLD;
            // Scaling
            attenuation = attenuation * (1.0 / (1 - CUTOFF_THRESHOLD));
            // Cannot drop below zero
            attenuation = max(attenuation, 0.0);*/

            // Inverse square falloff attenuation found in Unreal
            float attenuation = pow(clamp(1.0 - pow(distanceToLight / pointLight.radius, 4.0), 0.0, 1.0), 2.0) / (distanceToLight * distanceToLight + 1.0);

            vec3 radiance = (pointLight.color * pointLight.radiance) * attenuation;

            // Cook-Torrance BRDF
            float NDF = DistributionGGX(normal, H, roughness);   
            float G   = GeometrySmith(normal, V, L, roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
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
        }
    }

    // ambient lighting (we now use IBL as the ambient term)
   
    vec3 F = fresnelSchlickRoughness(max(dot(normal, V), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    vec3 irradiance = texture(irradianceCubeMap, normal).rgb;
    vec3 diffuse = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 R = reflect(-V, normal); 
    vec3 prefilteredColor = textureLod(prefilterCubeMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(normal, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambientLo = (kD * diffuse + specular) * ambientOcclusion;

    vec3 color = ambientLo + Lo;

    return color;
}

vec3 extractPositionFromDepth(float depth) {

    // Both UVs and depth are in the range of [0,1] so we move them to the range of [-1,1]
    float z = depth * 2.0 - 1.0;
    float w = (2.0 * projectionNear * projectionFar) / (projectionFar + projectionNear - z * (projectionFar - projectionNear));
    vec4 positionClipSpace = vec4(fragTextCoords * 2.0 - 1.0, z, 1.0);

    // Move back to view space
    vec4 positionViewSpace = inverseProjectionMatrix * positionClipSpace;

    // Perspective division
    positionViewSpace /= positionViewSpace.w;

    // Finally move back to world space
    return (inverseViewMatrix * positionViewSpace).xyz;
}

void main(void)
{
    // 1. Extract position, metallic map, normal, roughness map, albedo and ambient occlusion map values
    // from each of the buffers that compose the gBuffer
    float metallic = texture(gBufferMetallic, fragTextCoords).r;
    vec4 normalRoughness = texture(gBufferNormalRoughness, fragTextCoords);
    vec4 albedoAmbientOcclusion = texture(gBufferAlbedoAmbientOcclusion, fragTextCoords);
    
    vec3 position = extractPositionFromDepth(texture(gBufferDepth, fragTextCoords).x);
    vec3 normal = normalRoughness.rgb; 
    float roughness = normalRoughness.a;

    // Assuming the albedo is in sRGB space so we move it to linear RGB space 
    vec3 albedo = pow(albedoAmbientOcclusion.rgb, vec3(2.2)); 
    float ambientOcclusion = albedoAmbientOcclusion.a;

    // 2. Calculate color using PBR
    vec3 color = pbr(position, normal, albedo, metallic, roughness, ambientOcclusion);

    vec4 positionViewSpace = viewMatrix * vec4(position,1.0);
    float fragmentDepthViewSpace = -positionViewSpace.z;

    fragmentColor = vec4(color, 1.0);
}
