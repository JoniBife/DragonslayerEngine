#version 330 core

layout (lines) in; // [x,y,z,diameter]
layout (triangle_strip, max_vertices = 46) out;

out vec4 exPosition;
out vec3 exNormal;
out vec2 exTextCoord;
out vec3 tangFragPos;
out vec3 tangLightPos;
out vec3 tangViewPos;
out vec4 fragPosLightSpace;

// UNIFORMS
uniform vec3 lightPosition;
uniform vec3 viewPos;

uniform mat4 lightSpaceMatrix;
uniform mat3 normal;
uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

const int sectors = 6;

mat4 rotationFromDir(vec4 dir) {

    vec3 normalizedDir = normalize(dir.xyz);

    vec3 axis = cross(normalizedDir, vec3(0.0,1.0,0.0));
	float t = 1 + dot(normalizedDir, vec3(0.0,1.0,0.0));
	vec4 qtrn = normalize(vec4(t, -axis.x, axis.y, -axis.z));

    float xt = qtrn.y * qtrn.x;
	float xx = qtrn.y * qtrn.y;
	float xy = qtrn.y * qtrn.z;
	float xz = qtrn.y * qtrn.w;
	float yt = qtrn.z * qtrn.x;
	float yy = qtrn.z * qtrn.z;
	float yz = qtrn.z * qtrn.w;
	float zt = qtrn.w * qtrn.x;
	float zz = qtrn.w * qtrn.w;

	mat4 rot;

	rot[0][0] = 1.0 - 2.0 * (yy + zz);
	rot[0][1] = 2.0 * (xy + zt);
	rot[0][2] = 2.0 * (xz - yt);
	rot[0][3] = 0.0;
	
	rot[1][0] = 2.0 * (xy - zz);
	rot[1][1] = 1.0 - 2.0 * (xx + zz);
	rot[1][2] = 2.0 * (yz + xt);
	rot[1][3] = 0.0;
	
	rot[2][0] = 2.0 * (xz + yt);
	rot[2][1] = 2.0 * (yz - xt);
	rot[2][2] = 1.0 - 2.0 * (xx + yy);
	rot[2][3] = 0.0;
	
	rot[3][0] = 0.0;
	rot[3][1] = 0.0;
	rot[3][2] = 0.0;
	rot[3][3] = 1.0;

	return rot;
}

void generateCylinder(vec4 bottomPosition, vec4 topPosition, float bottomRadius, float topRadius)
{    
    const float PI = acos(-1.0);
    float sectorStep = (2 * PI / float(sectors));
    float sectorAngle;  // radian

    vec4 unitCircleVertices[sectors + 1]; 

    mat4 baseRotation = rotationFromDir(topPosition - bottomPosition);
    
    // Calculating the vertices of a circle centered in (0,0,0)
    for(int i = 0; i <= sectors; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices[i] = baseRotation * vec4(cos(sectorAngle) , 0.0 ,sin(sectorAngle), 0.0);
    }

    float u;
    vec4 v1, v2, v3, v4;
    vec3 n;
    vec2 uv1,uv2,uv3,uv4;
    vec4 fragPosition;

    n = vec3(0,1,0);

    v1 = (topPosition + vec4(unitCircleVertices[0].x * topRadius, unitCircleVertices[0].y * topRadius ,unitCircleVertices[0].z * topRadius, 0.0));
    v2 = topPosition;
    v3 = (topPosition + vec4(unitCircleVertices[1].x * topRadius, unitCircleVertices[1].y * topRadius ,unitCircleVertices[1].z * topRadius, 0.0));

    uv1 = vec2(unitCircleVertices[0].x * 0.5 + 0.5, -unitCircleVertices[0].z * 0.5 + 0.5);
    uv2 = vec2(0.5, 0.5);
    uv3 = vec2(unitCircleVertices[1].x * 0.5 + 0.5, -unitCircleVertices[1].z * 0.5 + 0.5);

    // Edges of the triangle : position delta
	vec4 deltaPos1 = v2 - v1;
	vec4 deltaPos2 = v3 - v1;

	// UV delta
	vec2 deltaUV1 = uv2 - uv1;
	vec2 deltaUV2 = uv3 - uv1;

    // Generating the tangents and bitangents ofr the whole face
	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	vec4 tangentAux = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    vec4 bitangentAux = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
	vec3 tangent = normalize(vec3(tangentAux.x, tangentAux.y, tangentAux.z));
	vec3 bitangent = normalize(vec3(bitangentAux.x, bitangentAux.y, bitangentAux.z));

    // Calculating the TBN matrix
    vec3 T = normalize(vec3(model * vec4(tangent,0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent,0.0)));
    vec3 N = normalize(vec3(model * vec4(n,0.0)));
    mat3 tbn = transpose(mat3(T, B, N)); // Tangent space matrix, converts vertices in world space to tangent space

    // Generating the top circle
    for(int i = 0; i <= sectors; ++i)
    {
        v1 = (topPosition + vec4(unitCircleVertices[i].x * topRadius, unitCircleVertices[i].y * topRadius,unitCircleVertices[i].z * topRadius, 0.0)); 
        gl_Position = projection *  view * model * v1; 
        exPosition = v1;
        exTextCoord = vec2(0.49, 0.485);//vec2(unitCircleVertices[i].x * 0.5 + 0.5, -unitCircleVertices[i].z * 0.5 + 0.5);
        exNormal = tbn * (normal * n);
        fragPosition = model * v1;
        tangFragPos = tbn * fragPosition.xyz;
	    tangLightPos = tbn * lightPosition.xyz;
	    tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex();
        v2 = topPosition;
        gl_Position = projection *  view * model *  v2; 
        exPosition = v2;
        exTextCoord = vec2(0.5, 0.5);
        exNormal = tbn * (normal * n);
        fragPosition = model * v2;
        tangFragPos = tbn * fragPosition.xyz;
	    tangLightPos = tbn * lightPosition.xyz;
	    tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex(); 
    }
    EndPrimitive();

    // Generating the side of the cylinder, one surface at a time
    for(int i = 0; i < sectors; ++i)
    {
        // Generate the vertices
        v1 = (bottomPosition + vec4(unitCircleVertices[i].x * bottomRadius, unitCircleVertices[i].y * bottomRadius ,unitCircleVertices[i].z * bottomRadius, 0.0));
        v2 = (topPosition + vec4(unitCircleVertices[i].x* topRadius, unitCircleVertices[i].y * topRadius ,unitCircleVertices[i].z* topRadius, 0.0));
        v3 = (bottomPosition + vec4(unitCircleVertices[i + 1].x * bottomRadius, unitCircleVertices[i + 1].y * bottomRadius ,unitCircleVertices[i + 1].z * bottomRadius, 0.0));
        v4 = (topPosition + vec4(unitCircleVertices[i + 1].x* topRadius, unitCircleVertices[i + 1].y * topRadius ,unitCircleVertices[i + 1].z* topRadius, 0.0));

        // Generate the texture coordinates
        u = float(i) / sectors; // u start at 0 increments to 1 as it reaches the last sector
        uv1 = vec2(u, 0.0);
        uv2 = vec2(u, 1.0);
        u = float((i+1)) / sectors;
        uv3 = vec2(u, 0.0);
        uv4 = vec2(u, 1.0);

        // Edges of the triangle : position delta
		vec4 deltaPos1 = v2 - v1;
		vec4 deltaPos2 = v3 - v1;

		// UV delta
		vec2 deltaUV1 = uv2 - uv1;
		vec2 deltaUV2 = uv3 - uv1;

        // Generating the tangents and bitangents ofr the whole face
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec4 tangentAux = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        vec4 bitangentAux = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
		vec3 tangent = normalize(vec3(tangentAux.x, tangentAux.y, tangentAux.z));
		vec3 bitangent = normalize(vec3(bitangentAux.x, bitangentAux.y, bitangentAux.z));

        // Calculating a single normal for the whole face
        n = normalize(cross(deltaPos1.xyz, deltaPos2.xyz));

        // Calculating the TBN matrix
        vec3 T = normalize(vec3(model * vec4(tangent,0.0)));
        vec3 B = normalize(vec3(model * vec4(bitangent,0.0)));
        vec3 N = normalize(vec3(model * vec4(n,0.0)));
        mat3 tbn = transpose(mat3(T, B, N)); // Tangent space matrix, converts vertices in world space to tangent space

        gl_Position = projection *  view * model * v1;
        exPosition = v1;
        exTextCoord = uv1;
        exNormal = tbn * (normal * n);
        fragPosition = model * v1;
        tangFragPos = tbn * fragPosition.xyz;
	    tangLightPos = tbn * lightPosition.xyz;
	    tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex(); 
        
        gl_Position = projection *  view * model * v2;
        exPosition = v2;
        exTextCoord = uv2; 
        exNormal = tbn * (normal * n);
        fragPosition = model * v2;
        tangFragPos = tbn * fragPosition.xyz;
	    tangLightPos = tbn * lightPosition.xyz;
	    tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex(); 

        gl_Position = projection *  view * model * v3;
        exPosition = v3;
        exTextCoord = uv3;
        exNormal = tbn * (normal * n);
        fragPosition = model * v3;
        tangFragPos = tbn * fragPosition.xyz;
	    tangLightPos = tbn * lightPosition.xyz;
	    tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex(); 
        
        gl_Position = projection *  view * model * v4;
        exPosition = v4;
        exTextCoord = uv4;
        exNormal = tbn * (normal * n);
        fragPosition = model * v4;
        tangFragPos = tbn * fragPosition.xyz;
        tangLightPos = tbn * lightPosition.xyz;
        tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex(); 
    }
    EndPrimitive();

    n = vec3(0,-1,0);

    v1 = bottomPosition;
    v2 = (bottomPosition + vec4(unitCircleVertices[0].x * bottomRadius, unitCircleVertices[0].y * bottomRadius ,unitCircleVertices[0].z * bottomRadius, 0.0));
    v3 = (bottomPosition + vec4(unitCircleVertices[1].x * bottomRadius, unitCircleVertices[1].y * bottomRadius ,unitCircleVertices[1].z * bottomRadius, 0.0));

    uv1 = vec2(0.5, 0.5);
    uv2 = vec2(unitCircleVertices[0].x * 0.5 + 0.5, -unitCircleVertices[0].z * 0.5 + 0.5);
    uv3 = vec2(unitCircleVertices[1].x * 0.5 + 0.5, -unitCircleVertices[1].z * 0.5 + 0.5);

    // Edges of the triangle : position delta
    deltaPos1 = v2 - v1;
	deltaPos2 = v3 - v1;

	// UV delta
	deltaUV1 = uv2 - uv1;
	deltaUV2 = uv3 - uv1;

    // Generating the tangents and bitangents ofr the whole face
	r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangentAux = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
    bitangentAux = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
	tangent = normalize(vec3(tangentAux.x, tangentAux.y, tangentAux.z));
	bitangent = normalize(vec3(bitangentAux.x, bitangentAux.y, bitangentAux.z));

    // Calculating the TBN matrix
    T = normalize(vec3(model * vec4(tangent,0.0)));
    B = normalize(vec3(model * vec4(bitangent,0.0)));
    N = normalize(vec3(model * vec4(n,0.0)));
    tbn = transpose(mat3(T, B, N)); // Tangent space matrix, converts vertices in world space to tangent space

    // Generating the bottom circle
    for(int i = 0; i <= sectors; ++i)
    {
        v1 = bottomPosition; 
        gl_Position = projection *  view * model * v1;   
        exPosition = v1;
        exTextCoord = vec2(0.5, 0.5);
        exNormal = tbn * (normal * n);
        fragPosition = model *v1;
        tangFragPos = tbn * fragPosition.xyz;
        tangLightPos = tbn * lightPosition.xyz;
        tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex(); 
        v2 = (bottomPosition + vec4(unitCircleVertices[i].x * bottomRadius, unitCircleVertices[i].y * bottomRadius ,unitCircleVertices[i].z * bottomRadius, 0.0));
        gl_Position = projection *  view * model * v2;
        exPosition = v2;
        exTextCoord = vec2(0.49, 0.485);//vec2(unitCircleVertices[i].x * 0.5 + 0.5, -unitCircleVertices[i].z * 0.5 + 0.5);;
        exNormal = tbn * (normal * n);
        fragPosition = model * v2;
        tangFragPos = tbn * fragPosition.xyz;
        tangLightPos = tbn * lightPosition.xyz;
        tangViewPos = tbn * viewPos;
        fragPosLightSpace = lightSpaceMatrix * fragPosition;
        EmitVertex();
    }
    EndPrimitive();
}

void main(void)
{
    vec4 bottomPosition = vec4(gl_in[0].gl_Position.xyz, 1.0);
    vec4 topPosition = vec4(gl_in[1].gl_Position.xyz, 1.0);
    
    float bottomRadius = gl_in[0].gl_Position.w;
    float topRadius = gl_in[1].gl_Position.w;
    
	generateCylinder(bottomPosition, topPosition, bottomRadius, topRadius);
}