#version 330 core

out vec4 fragmentColor;

void main()
{
    // Writing the depth in each of the color components to be able to visualize the depth map
    fragmentColor = vec4(
        gl_FragCoord.z,
        gl_FragCoord.z,
        gl_FragCoord.z, 
    1.0);
}