#version 330 core

layout (location = 0) out vec4 gBufferPositionMetallic; // Contains both the position and metallic values
layout (location = 1) out vec4 gBufferNormalRoughness; // Contains both the normal and roughness values
layout (location = 2) out vec4 gBufferAlbedoAmbientOcclusion; // Contains both the albedo and ambient occlusion values

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTextCoords;
in mat3 TBNMatrix;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D ambientOcclusionMap;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = (texture(normalMap, fragTextCoords).xyz * 2.0 - 1.0);

    vec3 Q1  = dFdx(fragPosition.xyz);
    vec3 Q2  = dFdy(fragPosition.xyz);
    vec2 st1 = dFdx(fragTextCoords);
    vec2 st2 = dFdy(fragTextCoords);

    vec3 N   = normalize(fragNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{    
    // Position = (X,Y,Z) and Metalliz = W
    gBufferPositionMetallic = vec4(
        fragPosition,
        texture(metallicMap, fragTextCoords).r
    );

    // Normal = (X,Y,Z) and Roughness = Ws
    gBufferNormalRoughness = vec4(
        getNormalFromMap(),
        texture(roughnessMap, fragTextCoords).r
    );

    // Albedo = (X,Y,Z) and Ambient Occlusion = W
    gBufferAlbedoAmbientOcclusion = vec4(
        texture(albedoMap, fragTextCoords).rgb,
        texture(ambientOcclusionMap, fragTextCoords).r
    );

    // gBuffer depth and stencil are done by OpenGL
}  