#version 330 core
out vec4 fragmentColor;

in vec3 fragTextCoord;

uniform samplerCube skybox;

void main()
{    
    fragmentColor = texture(skybox, fragTextCoord);
}