#version 330 core

layout (lines) in; // [x,y,z,diameter]
layout (triangle_strip, max_vertices = 46) out;

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

    vec3 exNormal = normal * vec3(0,0,0);

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

    vec4 v1, v2, v3, v4;

    // Generating the top circle
    for(int i = 0; i <= sectors; ++i)
    {
        v1 = (topPosition + vec4(unitCircleVertices[i].x * topRadius, unitCircleVertices[i].y * topRadius ,unitCircleVertices[i].z * topRadius, 0.0)); 
        gl_Position = lightSpaceMatrix * model * v1; 
        EmitVertex();
        v2 = topPosition;
        gl_Position = lightSpaceMatrix  * model * v2; 
        EmitVertex(); 
    }
    EndPrimitive();

    // Generating the side of the cylinder, one surface at a time
    for(int i = 0; i < sectors; ++i)
    {
        // Generate the vertices
        v1 = (bottomPosition + vec4(unitCircleVertices[i].x * bottomRadius, unitCircleVertices[i].y * bottomRadius ,unitCircleVertices[i].z * bottomRadius, 0.0));
        v2 = (topPosition + vec4(unitCircleVertices[i].x* topRadius, unitCircleVertices[i].y * topRadius ,unitCircleVertices[i].z* topRadius, 0.0));
        v3 = (bottomPosition + vec4(unitCircleVertices[i + 1].x * bottomRadius, unitCircleVertices[i].y * bottomRadius ,unitCircleVertices[i + 1].z * bottomRadius, 0.0));
        v4 = (topPosition + vec4(unitCircleVertices[i + 1].x* topRadius, unitCircleVertices[i].y * topRadius ,unitCircleVertices[i + 1].z* topRadius, 0.0));

        gl_Position = lightSpaceMatrix  * model * v1;
        EmitVertex(); 
        
        gl_Position = lightSpaceMatrix  * model * v2;
        EmitVertex(); 

        gl_Position = lightSpaceMatrix  * model * v3;
        EmitVertex(); 
        
        gl_Position = lightSpaceMatrix  * model * v4;
        EmitVertex(); 
    }
    EndPrimitive();

    // Generating the bottom circle
    for(int i = 0; i <= sectors; ++i)
    {
        v1 = bottomPosition; 
        gl_Position = lightSpaceMatrix  * model * v1;   
        EmitVertex(); 
        v2 = (bottomPosition + vec4(unitCircleVertices[i].x * bottomRadius, unitCircleVertices[i].y * bottomRadius ,unitCircleVertices[i].z * bottomRadius, 0.0));
        gl_Position = lightSpaceMatrix  * model * v2;
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