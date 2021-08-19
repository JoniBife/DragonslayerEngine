#version 330 core

layout (location = 0) out vec4 gBufferPositionMetallic; // Contains both the position and metallic values
layout (location = 1) out vec4 gBufferNormalRoughness; // Contains both the normal and roughness values
layout (location = 2) out vec4 gBufferAlbedoAmbientOcclusion; // Contains both the albedo and ambient occlusion values

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTextCoords;
in mat3 TBNMatrix;

uniform vec3  albedoTint;
uniform float normalStrength;
uniform float metallicFactor;
uniform float roughnessFactor;
uniform float aoFactor;

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
        texture(metallicMap, fragTextCoords).r * metallicFactor
    );

    // Normals in the normal map are colors so they were moved to the [0,1] range
    // however normals are in the [-1,1] range so we have to move them back this range
    vec3 normal = texture(normalMap, fragTextCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // Normal = (X,Y,Z) and Roughness = Ws
    gBufferNormalRoughness = vec4(
        normalize(TBNMatrix * normal) * normalStrength,
        texture(roughnessMap, fragTextCoords).r * roughnessFactor
    );

    // Albedo = (X,Y,Z) and Ambient Occlusion = W
    gBufferAlbedoAmbientOcclusion = vec4(
        pow(texture(albedoMap, fragTextCoords).rgb, vec3(2.2)) + albedoTint,
        texture(ambientOcclusionMap, fragTextCoords).r * aoFactor
    );

    // gBuffer depth and stencil are done by OpenGL
}  