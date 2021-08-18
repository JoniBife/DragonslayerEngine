#version 330 core

layout (location = 0) out vec4 gBufferPositionMetallic; // Contains both the position and metallic values
layout (location = 1) out vec4 gBufferNormalRoughness; // Contains both the normal and roughness values
layout (location = 2) out vec4 gBufferAlbedoAmbientOcclusion; // Contains both the albedo and ambient occlusion values

in vec3 fragPosition;
in vec2 fragTextCoords;
in mat3 TBNMatrix;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D ambientOcclusionMap;

void main()
{    
    // Position = (X,Y,Z) and Metalliz = W
    gBufferPositionMetallic = vec4(
        fragPosition,
        texture(metallicMap, fragTextCoords).r
    );

    // Normal = (X,Y,Z) and Roughness = Ws
    gBufferNormalRoughness = vec4(
        normalize(TBNMatrix * texture(normalMap, fragTextCoords).rgb),
        texture(roughnessMap, fragTextCoords).r
    );

    // Albedo = (X,Y,Z) and Ambient Occlusion = W
    gBufferAlbedoAmbientOcclusion = vec4(
        texture(albedoMap, fragTextCoords).rgb,
        texture(ambientOcclusionMap, fragTextCoords).r
    );

    // gBuffer depth and stencil are done by OpenGL
}  