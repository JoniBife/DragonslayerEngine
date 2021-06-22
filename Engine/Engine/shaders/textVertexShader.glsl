#version 330 core
in vec4 inPosition; // [pos.x, pos.y, textCoords.u ,textCoords.v]
out vec2 exTextCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    // No model matrix, it would be the same as having a model matrix which is the product identity
    gl_Position = projection * model * vec4(inPosition.xy, 0.0, 1.0);
    exTextCoords = inPosition.zw;
}  