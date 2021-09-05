#version 330 core

out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform float minEdgeThreshold;
uniform float maxEdgeThreshold;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

uniform bool enabled;

#define QUALITY(q) ((q) < 5 ? 1.0 : ((q) > 5 ? ((q) < 10 ? 2.0 : ((q) < 11 ? 4.0 : 8.0)) : 1.5))

uniform int iterations = 12;

/* Converts RGB colors to luma (which is a measure of the brightness)
and converts them from linear space to non linear space (to perceived brightness)
by using the inverse gamma transformation (x ^ (1/2.2)
*/
float rgbToLuma(vec3 rgb){
    return pow(dot(rgb, vec3(0.299, 0.587, 0.114)),1.0/2.2);
}

void main()
{

    vec3 currentColor = texture(previousRenderTexture, fragTextCoords.xy).rgb;

    if (enabled) {

        // EDGE DETECTION --------------------------------------------------

        // Sampling luma in a 3x3 range centered at the current fragment (excluding the corners)
        float lumaCenter = rgbToLuma(currentColor);
        float lumaUp = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy,ivec2(0,1)).rgb);
        float lumaDown = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(0,-1)).rgb);
        float lumaLeft = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(-1,0)).rgb);
        float lumaRight = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(1,0)).rgb);

        // Finding the maximum and minimum luma
        float maxLuma = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));
        float minLuma = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));
    
        // Finding the contrast between the maximum luma and minimum lum
        float lumaContrast = maxLuma - minLuma;

        // If the luma variation is lower that a threshold (or if we are in a really dark area), we are not on an edge, don't perform any AA.
        if(lumaContrast < max(minEdgeThreshold, maxLuma * maxEdgeThreshold)){
            fragmentColor = vec4(currentColor,1.0);
            return;
        } 

        // EDGE DIRECTION ---------------------------------------------------

        // Sampling lume in the leftover texels (the corners)
        float lumaDownLeft = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(-1,-1)).rgb);
        float lumaUpRight = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(1,1)).rgb);
        float lumaUpLeft = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(-1,1)).rgb);
        float lumaDownRight = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(1,-1)).rgb);

        // Compute an estimation of the gradient along the horizontal and vertical axis.
        float edgeHorizontal = 
                        abs((0.25 * lumaUpRight) + (-0.5 * lumaUp) + (0.25 * lumaUpLeft)) +
                        abs((0.50 * lumaRight ) + (-1.0 * lumaCenter) + (0.50 * lumaRight )) +
                        abs((0.25 * lumaDownRight) + (-0.5 * lumaDown) + (0.25 * lumaDownLeft));

        float edgeVertical = abs((0.25 * lumaUpRight) + (-0.5 * lumaRight) + (0.25 * lumaDownRight)) +
                         abs((0.50 * lumaUp ) + (-1.0 * lumaCenter) + (0.50 * lumaDown )) +
                         abs((0.25 * lumaUpLeft) + (-0.5 * lumaLeft) + (0.25 * lumaDownLeft));

        // Finding the line where the edge is (horizontal or vertical)
        bool isHorizontal = (edgeHorizontal >= edgeVertical);

        // Selecting the lumas to find the direction on a particular line (horizontal or vertical)
        float luma1 = isHorizontal ? lumaDown : lumaLeft;
        float luma2 = isHorizontal ? lumaUp : lumaRight;

        // Compute gradients in this direction.
        float gradient1 = luma1 - lumaCenter;
        float gradient2 = luma2 - lumaCenter;

        // Gradient in the corresponding direction, normalized.
        float gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));

        // Calculating step size (one texel)
        vec2 texelSize = 1.0 / textureSize(previousRenderTexture, 0);
        float stepLength = isHorizontal ? texelSize.x : texelSize.y;

        // Average luma in the correct direction.
        float lumaLocalAverage = 0.0;

        // Which direction is the steepest ?
        if(abs(gradient1) >= abs(gradient2)) {
            // Gradient1 indicates left or down, so the step has to be inverted
            stepLength = - stepLength;
            lumaLocalAverage = 0.5*(luma1 + lumaCenter);
        } else {
            lumaLocalAverage = 0.5*(luma2 + lumaCenter);
        }

        // Offset the UV by half a pixel to reach the edge
        vec2 currentUv = fragTextCoords.xy;

        if(isHorizontal) {
            currentUv.y += stepLength * 0.5;
        } else {
            currentUv.x += stepLength * 0.5;
        }

        // Calculate offset to move along the edge of the pixels
        vec2 offset = isHorizontal ? vec2(texelSize.x,0.0) : vec2(0.0,texelSize.y);

        // Compute UVs to explore on each side of the edge, orthogonally. The QUALITY allows us to step faster.
        vec2 uv1 = currentUv - offset;
        vec2 uv2 = currentUv + offset;

        // Read the lumas at both current extremities of the exploration segment, 
        // and compute the delta luma to the local average luma.
        float lumaEnd1 = rgbToLuma(texture(previousRenderTexture,uv1).rgb);
        float lumaEnd2 = rgbToLuma(texture(previousRenderTexture,uv2).rgb);
        lumaEnd1 -= lumaLocalAverage;
        lumaEnd2 -= lumaLocalAverage;

        // If the luma deltas at the current extremities are larger than the local gradient, 
        // we have reached the side of the edge.
        bool reached1 = abs(lumaEnd1) >= gradientScaled;
        bool reached2 = abs(lumaEnd2) >= gradientScaled;
        bool reachedBoth = reached1 && reached2;

        // If the side is not reached, we continue to explore in this direction.
        if(!reached1) {
            uv1 -= offset;
        }

        if(!reached2) {
            uv2 += offset;
        }   

        // If both sides have not been reached, continue to explore.
        if(!reachedBoth) {

            for(int i = 2; i < iterations; i++) {

                // If needed, read luma in 1st direction, compute delta.
                if(!reached1) {
                    lumaEnd1 = rgbToLuma(texture(previousRenderTexture, uv1).rgb);
                    lumaEnd1 = lumaEnd1 - lumaLocalAverage;
                }

                // If needed, read luma in opposite direction, compute delta.
                if(!reached2) {
                    lumaEnd2 = rgbToLuma(texture(previousRenderTexture, uv2).rgb);
                    lumaEnd2 = lumaEnd2 - lumaLocalAverage;
                }

                // If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
                reached1 = abs(lumaEnd1) >= gradientScaled;
                reached2 = abs(lumaEnd2) >= gradientScaled;
                reachedBoth = reached1 && reached2;

                // If the side is not reached, we continue to explore in this direction, with a variable quality.
                if(!reached1) {
                    uv1 -= offset * QUALITY(i);
                }

                if(!reached2) {
                    uv2 += offset * QUALITY(i);
                }

                // If both sides have been reached, stop the exploration.
                if(reachedBoth) { 
                    break;
                }
            }
        }

        // Compute the distances to each extremity of the edge.
        float distance1 = isHorizontal ? (fragTextCoords.x - uv1.x) : (fragTextCoords.y - uv1.y);
        float distance2 = isHorizontal ? (uv2.x - fragTextCoords.x) : (uv2.y - fragTextCoords.y);

        // In which direction is the extremity of the edge closer ?
        bool isDirection1 = distance1 < distance2;
        float distanceFinal = min(distance1, distance2);

        // Length of the edge.
        float edgeThickness = (distance1 + distance2);

        // UV offset: read in the direction of the closest side of the edge.
        float pixelOffset = - distanceFinal / edgeThickness + 0.5;

        // Is the luma at center smaller than the local average ?
        bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

        // If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
        // (in the direction of the closer side of the edge.)
        bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

        // If the luma variation is incorrect, do not offset.
        float finalOffset = correctVariation ? pixelOffset : 0.0;

        // Combine the four edges lumas (using intermediary variables for future computations with the same values).
        float lumaDownUp = lumaDown + lumaUp;
        float lumaLeftRight = lumaLeft + lumaRight;

        // Same for corners
        float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
        float lumaDownCorners = lumaDownLeft + lumaDownRight;
        float lumaRightCorners = lumaDownRight + lumaUpRight;
        float lumaUpCorners = lumaUpRight + lumaUpLeft;

        // Sub-pixel shifting
        // Full weighted average of the luma over the 3x3 neighborhood.
        float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
        // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
        float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaContrast,0.0,1.0);
        float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
        // Compute a sub-pixel offset based on this delta.
        float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * 0.75; // 0.75 is the subbixel quality

        // Pick the biggest of the two offsets.
        finalOffset = max(finalOffset,subPixelOffsetFinal);

        // Compute the final UV coordinates.
        vec2 finalUv = fragTextCoords;

        if(isHorizontal){
            finalUv.y += finalOffset * stepLength;
        } else {
            finalUv.x += finalOffset * stepLength;
        }

        // Read the color at the new UV coordinates, and use it.
        vec3 finalColor = texture(previousRenderTexture, finalUv).rgb;
        fragmentColor = vec4(finalColor,1.0);
        return;
        
    } else {
        fragmentColor = vec4(currentColor,1.0);
    }

    // HDR tonemapping
    //color = color / (color + vec3(1.0));
    // gamma correct
    //color = pow(color, vec3(1.0/2.2)); 
}
