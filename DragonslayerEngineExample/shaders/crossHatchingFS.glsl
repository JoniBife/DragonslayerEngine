#version 330 core
layout (location = 0) out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass
uniform sampler2D gBufferLocalPositionMetallic;
uniform sampler2D gBufferNormalRoughness;
uniform sampler2D[] crossHatchingLevels;

/* Converts RGB colors to luma (which is a measure of the brightness) */
float rgbToLuma(vec3 rgb){
    return dot(rgb, vec3(0.299, 0.587, 0.114));
}

vec4 biplanar(sampler2D sam, vec3 p, vec3 n, float k)
{
    // grab coord derivatives for texturing
    vec3 dpdx = dFdx(p);
    vec3 dpdy = dFdy(p);
    n = abs(n);

    // major axis (in x; yz are following axis)
    ivec3 ma = (n.x>n.y && n.x>n.z) ? ivec3(0,1,2) :
    (n.y>n.z)            ? ivec3(1,2,0) :
    ivec3(2,0,1) ;
    // minor axis (in x; yz are following axis)
    ivec3 mi = (n.x<n.y && n.x<n.z) ? ivec3(0,1,2) :
    (n.y<n.z)            ? ivec3(1,2,0) :
    ivec3(2,0,1) ;

    // median axis (in x;  yz are following axis)
    ivec3 me = ivec3(3) - mi - ma;

    // project+fetch
    vec4 x = textureGrad( sam, vec2(   p[ma.y],   p[ma.z]),
    vec2(dpdx[ma.y],dpdx[ma.z]),
    vec2(dpdy[ma.y],dpdy[ma.z]) );
    vec4 y = textureGrad( sam, vec2(   p[me.y],   p[me.z]),
    vec2(dpdx[me.y],dpdx[me.z]),
    vec2(dpdy[me.y],dpdy[me.z]) );

    // blend and return
    vec2 m = vec2(n[ma.x],n[me.x]);
    // optional - add local support (prevents discontinuty)
    m = clamp( (m-0.5773)/(1.0-0.5773), 0.0, 1.0 );
    // transition control
    m = pow( m, vec2(k/8.0) );
    return (x*m.x + y*m.y) / (m.x + m.y);
}

void main()
{
    vec3 color = texture(previousRenderTexture, fragTextCoords).xyz;
    vec3 localPosition = texture(gBufferLocalPositionMetallic, fragTextCoords).xyz;
    vec3 normal = texture(gBufferNormalRoughness, fragTextCoords).xyz;

    float luma = rgbToLuma(color); // [0,1]
    float crossHatchingL = luma * 4.0;
    int crossHatchingLevel = int(floor(crossHatchingL));

    vec3 crossHatching;
    if (crossHatchingLevel == 0) {
        crossHatching = mix(
            biplanar(crossHatchingLevels[2], localPosition * 3.0, normal, 8.0).xyz,
            biplanar(crossHatchingLevels[1], localPosition * 3.0, normal, 8.0).xyz,
            crossHatchingL
        );
    } else if (crossHatchingLevel == 1) {
        crossHatching = mix(
            biplanar(crossHatchingLevels[1], localPosition * 3.0, normal, 8.0).xyz,
            biplanar(crossHatchingLevels[0], localPosition * 3.0, normal, 8.0).xyz,
            crossHatchingL - 1.0
        );
    } else if (crossHatchingLevel >= 2) {
        crossHatching = mix(
            biplanar(crossHatchingLevels[0], localPosition * 3.0, normal, 8.0).xyz,
            vec3(1.0),
            crossHatchingL - 2.0
        );
    }

    fragmentColor = vec4(vec3(crossHatching.r) * luma,1.0);
}