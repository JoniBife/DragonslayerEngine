#version 330 core

out vec4 fragmentColor;

void main()
{
    gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth += gl_FrontFacing ? 0.01 : 0.0;

    // Writing the depth in each of the color components to be able to visualize the depth map
    fragmentColor = vec4(
        vec3(gl_FragCoord.z),
    1.0);
}