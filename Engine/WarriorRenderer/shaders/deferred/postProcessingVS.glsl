#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textCoords;

out vec2 fragTextCoords;

void main()
{
    fragTextCoords = textCoords;

    // We are drawing directly in clip space so no need to apply any transformations
    // The input vertices are already normalized device coordinates (i.e. in the range of -1 to 1)
    gl_Position = position; 
} 