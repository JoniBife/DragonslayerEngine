#version 330 core
out vec4 fragmentColor;

in vec3 fragTextCoord;

uniform samplerCube skybox;

void main()
{    
    vec3 skyboxColor = texture(skybox, fragTextCoord).rgb;

    skyboxColor = skyboxColor / (skyboxColor + vec3(1.0));
    skyboxColor = pow(skyboxColor, vec3(1.0/2.2));

    fragmentColor = vec4(skyboxColor, 1.0);
}