#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h>
#include "IDrawable.h"
#include "../math/Mat4.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../math/Vec2.h"

/*
* Usage example:
* 
* std::vector<Vec4> vertices = ...;
* std::vector<Vec4> colors = ...;
* 
* Mesh square(vertices,colors);
* 
* square.bind();
* 
* // Do stuff with the shader program (ex: set the uniforms);
* 
* square.draw();
* square.unbind();
* 
* delete square;
* 
*/
class Mesh : public IDrawable {

public:
	std::vector<Vec4> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec4> colors;
	std::vector<Vec2> textCoords;
	std::vector<GLubyte> indices;
	std::vector<Vec3> tangents;

private:
	GLuint vaoId = GLuint(0);
	GLuint vboId = GLuint(0);
	GLuint vboNormalsId = GLuint(0);
	GLuint vboColorsId = GLuint(0);
	GLuint vboTextCoordsId = GLuint(0);
	GLuint eboIndicesId = GLuint(0);
	GLuint vboTangentsId = GLuint(0);
	GLuint vboBitangentsId = GLuint(0);
	GLenum drawingPrimitive = GL_TRIANGLES;
	bool hasBeenInitialized = false;
	bool hasBeenBound = false;
	GLenum verticesBufferType = GL_STATIC_DRAW;
	int verticesBufferSize = -1;

public:

	Mesh();

	// Copy constructor
	Mesh(const Mesh& mesh);

	Mesh(const std::vector<Vec4>& vertices);

	Mesh(const std::vector<Vec4>& vertices,const std::vector<Vec4>& colors);

	Mesh(const std::vector<Vec4>& vertices,const std::vector<Vec4>& colors,const std::vector<GLubyte>& indices);

	Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals, const std::vector<Vec2>& textCoords);

	Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals);

	Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec2>& textCoords);
	
	// Deletes all the vbos, vaos and disables the vertex array atributes
	~Mesh() override;

	// Assignment
	Mesh& operator=(const Mesh& mesh);

	// Initializes the vao and vbo, required so that we can change the vertices after creating the mesh
	void init() override;

	// Binds the vertex array object with glBindArray
	void bind() override;
	// Unbinds the vertex array object with glBindArray(0)
	void unBind() override;

	// Draws the mesh using glDrawArrays
	void draw() override;

	// Calculates the tangents and bitangents
	void calculateTangents();

	// Creates a black centered in clip space (0,0,0)
	static Mesh* square(const float width = 0.75f);

	// Creates a black centered in clip space (0,0,0)
	static Mesh* rectangle(const float width = 1.0f, const float height = 0.75f);

	// Creates a black centered in clip space (0,0,0)
	static Mesh* triangle(const float width = 1.0f, const float height = 1.0f);

	// Loads the mesh from the file with the filePath
	static Mesh* loadFromFile(const std::string& filePath);

	// Applies the transformation matrix to all vertices of the mesh
	void transform(const Mat4& transformation);

	// Fills the colors list with color
	void paint(const Vec4& color);
        void setPrimitive(GLenum drawingPrimitive);

	void setVerticesBufferType(GLenum bufferType);

	void setVerticesBufferSize(int bufferSize);

	// Used to update the vertices when a buffer has already been created
	// If the number of vertices is the same it uses glBufferSubData
	// otherwise it allocates a new buffer to place the vertices and deletes the old one
	void updateVertices(const std::vector<Vec4>& vertices);
};

#endif
