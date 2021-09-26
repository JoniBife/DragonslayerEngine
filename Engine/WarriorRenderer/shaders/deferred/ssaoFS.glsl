#version 330 core
layout (location = 0) out vec4 fragmentColor;

// SSAO By John Chapman http://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html

in vec2 fragTextCoords;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

uniform sampler2D gBufferNormalRoughness; // Contains both the normal and roughness values
uniform sampler2D gBufferDepth;
uniform sampler2D noise;

// tile noise texture over screen, based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1366.0/4.0, 768.0/4.0);

uniform vec3 samples[64];
uniform uint sampleSize;

uniform float sampleRadius = 0.5;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform float projectionNear;
uniform float projectionFar;
uniform mat4 inverseProjectionMatrix;

vec3 extractPositionFromDepth(float depth) {

    // Both UVs and depth are in the range of [0,1] so we move them to the range of [-1,1]
    float z = depth * 2.0 - 1.0;
    float w = (2.0 * projectionNear * projectionFar) / (projectionFar + projectionNear - z * (projectionFar - projectionNear));
    vec4 positionClipSpace = vec4(fragTextCoords * 2.0 - 1.0, z, 1.0);

    // Move back to view space
    vec4 positionViewSpace = inverseProjectionMatrix * positionClipSpace;

    // Perspective division
    positionViewSpace /= positionViewSpace.w;

    return positionViewSpace.xyz;
}

mat3 calculateTBN(vec3 position, vec3 normal) {
    
    vec3 Q1  = dFdx(position);
    vec3 Q2  = dFdy(position);
    vec2 st1 = dFdx(fragTextCoords);
    vec2 st2 = dFdy(fragTextCoords);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return TBN;
}

void main()
{
    vec3 color = texture(previousRenderTexture, fragTextCoords).xyz;
    vec3 positionViewSpace = extractPositionFromDepth(texture(gBufferDepth, fragTextCoords).r);
    vec3 normal = texture(gBufferNormalRoughness, fragTextCoords).xyz;

    // Tiling the noise to repeat over the screen
    vec2 noiseTextCoords = fragTextCoords * noiseScale;
    vec3 randomVec = vec3(texture(noise, noiseTextCoords).xy, 0.0);

    vec3 T = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 B = cross(normal, T);
    mat3 TBN = mat3(T,B,normal); // Moves from tangent space to viewspace

    float occlusion = 0.0;
    for (unsigned int i = 0u; i < sampleSize; ++i) {
        
        vec3 sampleViewSpace = TBN * samples[i];
        vec4 samplePosition = vec4(positionViewSpace + sampleViewSpace * sampleRadius, 1.0);

        // Moving to clip space
        samplePosition = projectionMatrix * samplePosition;

        // Transforming to NDC by performing perspective division
        samplePosition /= samplePosition.w;

        // Moving to the [0, 1] range
        samplePosition = samplePosition*0.5 + 0.5;

        vec2 sampleTextCoords = samplePosition.xy;

        float sampleDepth = extractPositionFromDepth(texture(gBufferDepth, sampleTextCoords).r).z;

        // Comparing depth in view space, increasing in the -z direction
        float rangeCheck = smoothstep(0.0, 1.0, sampleRadius / abs(positionViewSpace.z  - sampleDepth));
        occlusion += (sampleDepth >= positionViewSpace.z + 0.025 ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion /= sampleSize;

    fragmentColor = vec4(color * (1.0 - occlusion), 1.0);
} 
