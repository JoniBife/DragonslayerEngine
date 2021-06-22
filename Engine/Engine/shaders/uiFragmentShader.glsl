#version 330 core
in vec2 exTextCoord;
out vec4 fragmentColor;

uniform sampler2D text;

void main()
{    
    fragmentColor = texture(text,exTextCoord);
} 