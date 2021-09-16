#version 330 core

out vec4 fragmentColor;

in vec2 fragTextCoords;

uniform float minEdgeThreshold;
uniform float maxEdgeThreshold;

uniform sampler2D previousRenderTexture; // A texture that resulted from the previous render pass

uniform bool enabled;

const float quality[] = float[](1.0, 1.0, 1.0, 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0);

uniform int iterations = 11;

/* Converts RGB colors to luma (which is a measure of the brightness) */
float rgbToLuma(vec3 rgb){
    return dot(rgb, vec3(0.299, 0.587, 0.114));
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
    
        // Finding the contrast between the maximum luma and minimum luma
        float lumaContrast = maxLuma - minLuma;

        // If the luma variation is lower that a threshold (or if we are in a really dark area), we are not on an edge, don't perform any AA.
        if(lumaContrast < max(minEdgeThreshold, maxLuma * maxEdgeThreshold)){
            fragmentColor = vec4(currentColor,1.0);
            return;
        } 
        // DEBUG Edge detection
        /* else { 
            currentColor.r += 0.5;
            fragmentColor = vec4(currentColor,1.0);
            return;
        }*/

        // EDGE DIRECTION --------------------------------------------------

        // Sampling lume in the leftover texels (the corners)
        float lumaUpRight = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(1,1)).rgb);
        float lumaUpLeft = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(-1,1)).rgb);
        float lumaDownLeft = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(-1,-1)).rgb);
        float lumaDownRight = rgbToLuma(textureOffset(previousRenderTexture, fragTextCoords.xy, ivec2(1,-1)).rgb);

        // Compute an estimation of the gradient along the horizontal and vertical axis.
        float edgeVertical = 
                        abs((0.25 * lumaUpRight) + (-0.5 * lumaUp) + (0.25 * lumaUpLeft)) +
                        abs((0.50 * lumaRight ) + (-1.0 * lumaCenter) + (0.50 * lumaRight )) +
                        abs((0.25 * lumaDownRight) + (-0.5 * lumaDown) + (0.25 * lumaDownLeft));

        float edgeHorizontal = abs((0.25 * lumaUpRight) + (-0.5 * lumaRight) + (0.25 * lumaDownRight)) +
                         abs((0.50 * lumaUp ) + (-1.0 * lumaCenter) + (0.50 * lumaDown )) +
                         abs((0.25 * lumaUpLeft) + (-0.5 * lumaLeft) + (0.25 * lumaDownLeft));

        // Finding the line where the edge is (horizontal or vertical)
        bool isEdgeHorizontal = (edgeHorizontal >= edgeVertical);

        // DEBUG Edge direction
        /*if (isHorizontal) {
            currentColor.r += 0.5;
            fragmentColor = vec4(currentColor,1.0);
            return;
        } else {
            currentColor.b += 0.5;
            fragmentColor = vec4(currentColor,1.0);
            return;
        }*/

        // EDGE EXPLORATION --------------------------------------------------

        // Selecting the lumas to find the direction on a particular line (horizontal or vertical)
        // If the edge is horizontal we move up or down, if the edge is vertical we move left or right

        float luma1; //= isEdgeHorizontal ? lumaDown : lumaLeft;
        float luma2; //= isEdgeHorizontal ? lumaUp : lumaRight;
        float gradientScaled;
        float stepLength;
        float lumaLocalAverage;
        vec2 edgeUv;
        vec2 offset;
        
        if (isEdgeHorizontal) {
            luma1 = lumaDown;
            luma2 = lumaUp;

            // Compute gradients in this direction.
            float gradient1 = luma1 - lumaCenter;
            float gradient2 = luma2 - lumaCenter;

            // Gradient in the corresponding direction, normalized.
            gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));

            // Calculating step size (one texel)
            // If the edge is horizontal we move up or down, if the edge is vertical we move left or right
            vec2 texelSize = 1.0 / textureSize(previousRenderTexture, 0);
            stepLength = texelSize.y;

            // Average luma in the correct direction.
            lumaLocalAverage = 0.0;

            // Which direction has the largest luma contrast from the center?
            if(abs(gradient1) >= abs(gradient2)) {
                // Gradient1 indicates left or down, so the step has to be inverted
                stepLength = - stepLength;
                lumaLocalAverage = 0.5*(luma1 + lumaCenter);
            } else {
                lumaLocalAverage = 0.5*(luma2 + lumaCenter);
            }

            // Move the UV by half a pixel towards the edge, to reach the edge
            // If the edge is horizontal we move up or down, if the edge is vertical we move left or right
            edgeUv = fragTextCoords.xy;
            edgeUv.y += stepLength * 0.5;

            // Calculate offset to move along the edge of the pixels
            offset = vec2(texelSize.x,0.0);

        } else {
            luma1 = lumaLeft;
            luma2 = lumaRight;

            // Compute gradients in this direction.
            float gradient1 = luma1 - lumaCenter;
            float gradient2 = luma2 - lumaCenter;

            // Gradient in the corresponding direction, normalized.
            gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));

            // Calculating step size (one texel)
            // If the edge is horizontal we move up or down, if the edge is vertical we move left or right
            vec2 texelSize = 1.0 / textureSize(previousRenderTexture, 0);
            stepLength = texelSize.x;

            // Average luma in the correct direction.
            lumaLocalAverage = 0.0;

            // Which direction has the largest luma contrast from the center?
            if(abs(gradient1) >= abs(gradient2)) {
                // Gradient1 indicates left or down, so the step has to be inverted
                stepLength = - stepLength;
                lumaLocalAverage = 0.5*(luma1 + lumaCenter);
            } else {
                lumaLocalAverage = 0.5*(luma2 + lumaCenter);
            }

            // Move the UV by half a pixel towards the edge, to reach the edge
            // If the edge is horizontal we move up or down, if the edge is vertical we move left or right
            edgeUv = fragTextCoords.xy;
            edgeUv.x += stepLength * 0.5;

            // Calculate offset to move along the edge of the pixels
            offset = vec2(0.0,texelSize.y);
        }

        bool reached1 = false; 
        bool reached2 = false;
        vec2 uv1 = edgeUv; // Uv coords of the down or left exploration
        vec2 uv2 = edgeUv; // Uv coords of the up or right exploration
        float deltaLuma1, deltaLuma2; // The luma contrast between the edge and the extremeties

        for(int i = 0; i < iterations; i++) {

            // If the segment extremity has not been reached then continue
            // advancing and computing the deltaLuma
            if(!reached1) {
                uv1 -= offset * quality[i];
                deltaLuma1 = rgbToLuma(textureLod(previousRenderTexture,uv1, 0).rgb) - lumaLocalAverage;

                // If the luma deltas at the current extremities are larger than the local gradient, 
                // we have reached the side of the edge.
                reached1 = abs(deltaLuma1) >= gradientScaled;
            }

            // If the segment extremity has not been reached then continue
            // advancing and computing the deltaLuma
            if(!reached2) {
                uv2 += offset * quality[i];
                deltaLuma2 = rgbToLuma(textureLod(previousRenderTexture,uv2, 0).rgb) - lumaLocalAverage;

                // If the luma deltas at the current extremities are larger than the local gradient, 
                // we have reached the side of the edge.
                reached2 = abs(deltaLuma2) >= gradientScaled;
            }

            // Stop as soon as we reach both extremities
            if (reached1 && reached2) break;
        }

        // Compute the distances to each extremity of the edge.
        float distance1 = isEdgeHorizontal ? (fragTextCoords.x - uv1.x) : (fragTextCoords.y - uv1.y);
        float distance2 = isEdgeHorizontal ? (uv2.x - fragTextCoords.x) : (uv2.y - fragTextCoords.y);

        // Is the luma at center smaller than the local average ?
        bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;
        
        // Finding the distance to the closest extremity and the delta luma curresponding to that delta
        float smallestDistanceToExtremity;
        float smallestLumaToExtremity;

        if ( distance1 < distance2) {
            smallestDistanceToExtremity = distance1;
            smallestLumaToExtremity = deltaLuma1;
        } else {
            smallestDistanceToExtremity = distance2;
            smallestLumaToExtremity = deltaLuma2;
        }

        // Checking if the luma variations are coherent
        // If the delta luma in the extremity has a different sign than the delta luma from the center to the edge 
        // then we explored too much and have maybe reached another edge
        bool exploredTooMuch = sign(smallestLumaToExtremity) != sign(lumaLocalAverage - lumaCenter);

        // If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
        // (in the direction of the closer side of the edge.)
        //bool isDirection1 = distance1 < distance2;
        //bool correctVariation = ((isDirection1 ? deltaLuma1 : deltaLuma2) < 0.0) != isLumaCenterSmaller;

        float finalOffset = 0.0;

        // If the luma variation is incorrect, do not offset.
        if (!exploredTooMuch) {
            // Length of the edge.
            float edgeLength = (distance1 + distance2);

            // UV offset: read in the direction of the closest side of the edge.
            float texelOffset = - smallestDistanceToExtremity / edgeLength + 0.5;

            finalOffset = texelOffset;
        }

        // SUBPIXEL ANTI-ALIASING --------------------------------------------------

        // Sub-pixel shifting

        // Combine the four edges lumas (using intermediary variables for future computations with the same values).
        float lumaDownUp = lumaDown + lumaUp;
        float lumaLeftRight = lumaLeft + lumaRight;

        // Same for corners
        float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
        float lumaDownCorners = lumaDownLeft + lumaDownRight;
        float lumaRightCorners = lumaDownRight + lumaUpRight;
        float lumaUpCorners = lumaUpRight + lumaUpLeft;

        // Full weighted average of the luma over the 3x3 neighborhood.
        float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
        // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
        float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaContrast,0.0,1.0);
        float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
        // Compute a sub-pixel offset based on this delta.
        float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * 0.75; // 0.75 is the subpixel quality

        // Pick the biggest of the two offsets.
        finalOffset = max(finalOffset,subPixelOffsetFinal);

        // BILINEAR FILTERING --------------------------------------------------

        // Compute the final UV coordinates.
        // The color will be sampled using a bilinear filtering (i.e. slightly blured)
        vec2 finalUv = fragTextCoords;

        if(isEdgeHorizontal){
            finalUv.y += finalOffset * stepLength;
        } else {
            finalUv.x += finalOffset * stepLength;
        }

        // Read the color at the new UV coordinates, and use it.
        fragmentColor = vec4(texture(previousRenderTexture, finalUv).rgb,1.0);
    } else {
        fragmentColor = vec4(currentColor,1.0);
    }
      
}
