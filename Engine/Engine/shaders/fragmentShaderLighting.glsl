#version 330 core

// INPUT
in vec4 exPosition;
in vec3 exNormal;
in vec2 exTextCoord;
in vec4 fragPos;

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
	float spec = pow(max(dot(normalizedNormal, halfwayDir), 0.0), shininess * 4.0);
	vec3 specular = specularStrength * spec * color;
	/*/ // Phong
	vec3 reflectDir = reflect(-lightDir, normalizedNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor; 
	/**/

	// Combining all three components
	return ambient + diffuse + specular;
}

void main(void)
{
	vec3 result = calculateLight(lightColor, exNormal, vec3(fragPos), lightPosition, viewPos);

	fragmentColor = vec4(result * exColor, 1.0f);
}

















