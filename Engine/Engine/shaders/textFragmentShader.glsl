#version 330 core
in vec2 exTextCoords;
out vec4 fragmentColor;

uniform sampler2D text; // mono-colored bitmap image of the glyph, the texture info is only stored in the red component
uniform vec4 textColor;

void main()
{    
    fragmentColor = vec4(textColor.xyz, texture(text, exTextCoords).r * textColor.w);
} 