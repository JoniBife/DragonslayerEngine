#version 330 core

// INPUT
in vec4 exPosition;
in vec4 exColor;
in vec3 exNormal;
in vec2 exTextCoord;
in vec4 fragPos;
in vec4 fragPosLightSpace;

out vec4 fragmentColor;

// UNIFORMS
uniform vec3 lightPosition;
uniform vec3 viewPos;

// Ambient Lighting
uniform vec3 lightColor;
uniform float ambientStrength;

// Specular Lighting
uniform float specularStrength;
uniform uint shininess;

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;

float calculateShadows(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
	// Perspective division
	vec3 projCoords = fragPosLightSpace.xyz/fragPosLightSpace.w; // Actually meaningless since the proj matrix used for light space is orthographic
	// Moving the coords to [0,1] which is the range of the depth map
	projCoords = projCoords * 0.5 + 0.5;
	// Closest depth from the light's point of view
	float closestDepth = texture(shadowMap, projCoords.xy).r;   
	// Current depth from the light's point of view
	float currentDepth = projCoords.z;  

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	// PCF Filter
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(currentDepth > 1.0)
        return 0.0f;

	return shadow;
}

vec3 calculateLight(vec3 color, vec3 surfaceNormal, vec3 fragPos, vec3 lightPos, vec3 viewPos) {

	// Ambient Lighting
	vec3 ambient = ambientStrength * color;

	// Diffuse Lighting 
	vec3 normalizedNormal = normalize(surfaceNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuseImpact = max(dot(lightDir, normalizedNormal), 0.0);
	vec3 diffuse = diffuseImpact * color;

	// Specular Lighting
	vec3 viewDir = normalize(viewPos - fragPos);
	/**/ // Blinn 
	vec3 halfwayDir = normalize(lightDir + viewDir);
	 // When using the blinn model the angle is usually smaller, so we increase the shininess to match the result of the phong model
	float spec = pow(max(dot(normalizedNormal, halfwayDir), 0.0), float(shininess) * 4.0);
	vec3 specular = specularStrength * spec * color;
	/*/ // Phong
	vec3 reflectDir = reflect(-lightDir, normalizedNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor; 
	/**/

	// Shadow
	float shadow = calculateShadows(fragPosLightSpace, surfaceNormal, lightDir);

	// Combining all four components
	return ambient + (1 - shadow) * (diffuse + specular);
}

void main(void)
{
	vec3 texColor = texture(diffuseMap, exTextCoord * 2).rgb;

	vec3 result = calculateLight(lightColor, exNormal, vec3(fragPos), lightPosition, viewPos);

	fragmentColor = vec4(result * texColor, 1.0);
}

















