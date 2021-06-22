#version 330 core

// INPUT
in vec4 exPosition;
in vec3 exNormal;
in vec2 exTextCoord;
in vec3 tangFragPos;
in vec3 tangLightPos;
in vec3 tangViewPos;

out vec4 fragmentColor;

// Ambient Lighting
uniform vec3 lightColor;
uniform float ambientStrength;

// Specular Lighting
uniform float specularStrength;
uniform uint shininess;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform bool normalMapping;

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

	return ambient + diffuse + specular;
}

void main(void)
{
	// Multiplying texture coordinate by the texture coefficient to scale its size
	vec2 textCoord = exTextCoord * 2;

	// Texture color
	vec3 colorTxt = texture(diffuseMap, textCoord).rgb;

	vec3 normal;
	if (normalMapping) {
		// Texture normal, loaded from normal map
		normal = texture(normalMap, textCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0);   
	} else {
		normal = exNormal;
	}

	vec3 result = calculateLight(lightColor, normal, tangFragPos, tangLightPos, tangViewPos);

	fragmentColor = vec4(result * colorTxt, 1.0);
}

















