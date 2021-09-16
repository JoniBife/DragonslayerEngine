#version 330 core

layout (location = 0) out float gBufferMetallic; // Contains the metallic value
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
    // Metallic = X
    gBufferMetallic =  texture(metallicMap, fragTextCoords).r * metallicFactor;

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
        texture(albedoMap, fragTextCoords).rgb + albedoTint,
        texture(ambientOcclusionMap, fragTextCoords).r * aoFactor
    );

    // gBuffer depth and stencil are done by OpenGL
}  