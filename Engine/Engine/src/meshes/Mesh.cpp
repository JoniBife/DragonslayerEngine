#include "Mesh.h"
#include "../utils/ColorRGBA.h"
#include "../utils/OpenGLUtils.h"
#include <iostream>
#include "../math/Vec2.h"
#include "MeshLoader.h"
#include <assert.h>
#include <vector>

#define VERTICES 0
#define NORMALS 1
#define COLORS 2
#define TEXTCOORDS 3
#define TANGENTS 4

Mesh::Mesh() {}

Mesh::Mesh(const Mesh& mesh) {
	vertices = mesh.vertices;
	colors = mesh.colors;
	normals = mesh.normals;
	textCoords = mesh.textCoords;
	indices = mesh.indices;
	tangents = mesh.tangents;
}

Mesh::Mesh(const std::vector<Vec4>& vertices) : vertices(vertices) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec4>& colors) : vertices(vertices), colors(colors) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec4>& colors, const std::vector<GLubyte>& indices) :
	vertices(vertices), colors(colors), indices(indices) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals, const std::vector<Vec2>& textCoords)
	: vertices(vertices), normals(normals), textCoords(textCoords) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals)
	: vertices(vertices), normals(normals) {
}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec2>& textCoords)
	: vertices(vertices), textCoords(textCoords) {}

// Deletes all the vbos, vaos and disables the vertex array atributes
Mesh::~Mesh() {

	if (!hasBeenInitialized) {
		return;
	}

	// Bind the the vao so that we can disable the vertex attrib array
	GL_CALL(glBindVertexArray(vaoId));
	GL_CALL(glDisableVertexAttribArray(VERTICES));
	GL_CALL(glDisableVertexAttribArray(NORMALS));
	GL_CALL(glDisableVertexAttribArray(COLORS));
	GL_CALL(glDisableVertexAttribArray(TEXTCOORDS));
	GL_CALL(glDisableVertexAttribArray(TANGENTS));
	GL_CALL(glDeleteBuffers(1, &vboId));
	if (!normals.empty())
		GL_CALL(glDeleteBuffers(1, &vboNormalsId));
	if (!colors.empty())
		GL_CALL(glDeleteBuffers(1, &vboColorsId));
	if (!textCoords.empty())
		GL_CALL(glDeleteBuffers(1, &vboTextCoordsId));
	if (!indices.empty()) {
		GL_CALL(glDeleteBuffers(1, &eboIndicesId));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	if (!tangents.empty()) {
		GL_CALL(glDeleteBuffers(1, &vboTangentsId));
	}
	GL_CALL(glDeleteVertexArrays(1, &vaoId));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindVertexArray(0));
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	vertices = mesh.vertices;
	colors = mesh.colors;
	normals = mesh.normals;
	textCoords = mesh.textCoords;
	indices = mesh.indices;
	return *this;
}

// Initializes the vao and vbos, required so that we can change the vertices after creating the mesh
void Mesh::init() {
	if (hasBeenInitialized) {
		return;
	}

	GL_CALL(glGenVertexArrays(1, &vaoId));
	GL_CALL(glBindVertexArray(vaoId));
	{
		// Obtaining the number of buffers that need to be created
		GLsizei numberOfBuffers = 1;
		int idxNormals, idxColors, idxTextCoords, idxIndices, idxTangents;
		if (!normals.empty()) {
			idxNormals = numberOfBuffers;
			++numberOfBuffers;
		}
		if (!colors.empty()) {
			idxColors = numberOfBuffers;
			++numberOfBuffers;
		}
		if (!textCoords.empty()) {
			idxTextCoords = numberOfBuffers;
			++numberOfBuffers;
		}
		if (!indices.empty()) {
			idxIndices = numberOfBuffers;
			++numberOfBuffers;
		}
		if (!tangents.empty()) {
			idxTangents = numberOfBuffers;
			++numberOfBuffers;
		}

		// Allocated on the heap because the numberOfBuffers is only known on run-time
		GLuint* bufferIds = new GLuint[numberOfBuffers];

		// Generating all buffers at once, its better than generating each of them separately 
		GL_CALL(glGenBuffers(numberOfBuffers, bufferIds));

		vboId = bufferIds[0];
		// Binding the vertices to the first vbo
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));
		{
			// If the user specified a custom size for the vertices buffer then it must be larger or equal to the number of vertices
			if (verticesBufferSize > -1)
				assert(vertices.size() <= verticesBufferSize);

			// The spec ensures that vectors store their elements contiguously
			// https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
			GL_CALL(glBufferData(GL_ARRAY_BUFFER,
				verticesBufferSize == -1 ? vertices.size() * sizeof(Vec4) : verticesBufferSize * sizeof(Vec4),
				vertices.size() == 0 ? NULL : &vertices[0], verticesBufferType));
			GL_CALL(glEnableVertexAttribArray(VERTICES));
			GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
		}

		if (!normals.empty()) {
			vboNormalsId = bufferIds[idxNormals];
			// Binding the colors to the second vbo
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId));
			{
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3), &normals[0], GL_STATIC_DRAW));
				GL_CALL(glEnableVertexAttribArray(NORMALS));
				GL_CALL(glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0));
			}
		}


		if (!colors.empty()) {
			vboColorsId = bufferIds[idxColors];
			// Binding the colors to the second vbo
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboColorsId));
			{
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vec4), &colors[0], GL_STATIC_DRAW));
				GL_CALL(glEnableVertexAttribArray(COLORS));
				GL_CALL(glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
			}
		}

		if (!textCoords.empty()) {
			vboTextCoordsId = bufferIds[idxTextCoords];
			// Binding the colors to the second vbo
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboTextCoordsId));
			{
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, textCoords.size() * sizeof(Vec2), &textCoords[0], GL_STATIC_DRAW));
				GL_CALL(glEnableVertexAttribArray(TEXTCOORDS));
				GL_CALL(glVertexAttribPointer(TEXTCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0));
			}
		}

		// If this mesh was created with indices then they will be placed in an element array buffer
		if (!indices.empty()) {
			eboIndicesId = bufferIds[idxIndices];
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndicesId));
			{
				GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), &indices[0], GL_STATIC_DRAW));
			}
		}

		if (!tangents.empty()) {
			vboTangentsId = bufferIds[idxTangents];

			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboTangentsId));
			{
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vec3), &tangents[0], GL_STATIC_DRAW));
				GL_CALL(glEnableVertexAttribArray(TANGENTS));
				GL_CALL(glVertexAttribPointer(TANGENTS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0));
			}
		}

		// BufferIds was allocated on the heap therefore we delete it because we no longer need it
		delete[] bufferIds;
	}
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	if (!indices.empty()) {
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	hasBeenInitialized = true;
}

// Binds the vertex array object with glBindArray
void Mesh::bind() {
	assert(hasBeenInitialized);
	GL_CALL(glBindVertexArray(vaoId));
	hasBeenBound = true;
}
// Unbinds the vertex array object with glBindArray(0)
void Mesh::unBind() {
	GL_CALL(glBindVertexArray(0));
}

// Draws the mesh using glDrawArrays
void Mesh::draw() {
	assert(hasBeenInitialized && hasBeenBound);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (!indices.empty()) {
		GL_CALL(glDrawElements(drawingPrimitive, GLsizei(indices.size()), GL_UNSIGNED_BYTE, (GLvoid*)0));
	}
	else
	{
		GL_CALL(glDrawArrays(drawingPrimitive, 0, GLsizei(vertices.size())));
	}
}

void Mesh::calculateTangents() {
	// TODO Calculate the tangents and bitangents in parallel after a certain number of vertices
	for (int i = 0; i < vertices.size(); i += 3) {
		// Shortcuts for vertices
		Vec4& v0 = vertices[i + 0];
		Vec4& v1 = vertices[i + 1];
		Vec4& v2 = vertices[i + 2];

		// Shortcuts for UVs
		Vec2& uv0 = textCoords[i + 0];
		Vec2& uv1 = textCoords[i + 1];
		Vec2& uv2 = textCoords[i + 2];

		// Edges of the triangle : position delta
		Vec4 deltaPos1 = v1 - v0;
		Vec4 deltaPos2 = v2 - v0;

		// UV delta
		Vec2 deltaUV1 = uv1 - uv0;
		Vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vec4 tangentAux = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		Vec3 tangent = Vec3(tangentAux.x, tangentAux.y, tangentAux.z);
		tangent.normalize();

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);
	}

}

// Creates a black square centered in clip space (0,0,0)
Mesh* Mesh::square(const float width) {
	Mesh* square = new Mesh();

	square->vertices = {
		// first triangle
		{-width / 2, -width / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -width / 2, 0.0f, 1.0f}, // bottom right vertex
		{width / 2, width / 2, 0.0f, 1.0f}, // top right vertex
		// second triangle
		{-width / 2, width / 2, 0.0f, 1.0f}, // top left vertex
		{-width / 2, -width / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, width / 2, 0.0f, 1.0f} // top right vertex
	};

	return square;
}

// Creates a black rectangle centered in clip space (0,0,0)
Mesh* Mesh::rectangle(const float width, const float height) {
	Mesh* rectangle = new Mesh();

	rectangle->vertices = {
		// first triangle
		{-width / 2, height / 2, 0.0f, 1.0f}, // top left vertex
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex

		// second triangle
		{-width / 2, height / 2, 0.0f, 1.0f}, // top left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
		{width / 2, height / 2, 0.0f, 1.0f} // top right vertex
	};

	rectangle->textCoords = {
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f}
	};

	return rectangle;

}

// Creates a black triangle centered in clip space (0,0,0)
Mesh* Mesh::triangle(const float width, const float height) {
	Mesh* triangle = new Mesh();

	triangle->vertices = {
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
		{0, height / 2, 0.0f, 1.0f} // top center vertex		
	};

	return triangle;
}

Mesh* Mesh::loadFromFile(const std::string& filePath) {
	return MeshLoader::loadFromFile(filePath);
}

void Mesh::transform(const Mat4& transformation) {
	for (Vec4& vert : vertices) {
		vert = transformation * vert;
	}
}

void Mesh::paint(const Vec4& color) {
	if (colors.empty()) {
		int colorSize = int(vertices.size());

		for (int i = 0; i < colorSize; i++) {
			colors.push_back(color);
		}
	}
	else {
		for (Vec4& vec : colors) {
			vec = color;
		}
	}
}

void Mesh::setPrimitive(GLenum drawingPrimitive) {
	this->drawingPrimitive = drawingPrimitive;
}

void Mesh::setVerticesBufferType(GLenum bufferType) {
	this->verticesBufferType = bufferType;
}

void Mesh::setVerticesBufferSize(int bufferSize) {
	this->verticesBufferSize = bufferSize;
}

void Mesh::updateVertices(const std::vector<Vec4>& vertices) {

	GL_CALL(glBindVertexArray(vaoId));
	{
		if (vertices.size() <= this->vertices.size() || vertices.size() <= verticesBufferSize) {

			this->vertices = vertices;

			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));
			{
				GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vec4), &this->vertices[0]));
			}
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
		else {

			this->vertices = vertices;

			// Deleting old buffer
			GL_CALL(glDeleteBuffers(1, &vboId));

			// Generating a new one
			GL_CALL(glGenBuffers(1, &vboId));
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));
			{
				GL_CALL(glBufferData(GL_ARRAY_BUFFER,
					verticesBufferSize == -1 ? this->vertices.size() * sizeof(Vec4) : verticesBufferSize * sizeof(Vec4),
					&this->vertices[0], verticesBufferType));
				GL_CALL(glEnableVertexAttribArray(VERTICES));
				GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
			}
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}
	}
	GL_CALL(glBindVertexArray(0));
}