#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include "../GLObject.h"

/*
* Usage example:
* 
* // Creating a vertex shader
* Shader s(GL_VERTEX_SHADER, "C:/Dev/CGJ-AVT/Engine/Engine/shaders/vertexShader.glsl");
* 
* // Obtaining the vertex shader id
* s.getId();
* 
* // Printing the id, the type, the code and the filepath of the shader
* std::cout << s << std::endl;
* 
* // Deleting shader
* delete s;
* 
*/
class Shader : public GLObject {

private:
	std::string filePath;
	GLuint id = 0u;
	GLenum type;
	std::string code;

public:
	// Default constructor
	Shader();
	// Receives the shader file path and compiles the shader
	Shader(const GLenum shaderType, const std::string& filePath);
	// Deletes the shader using glDeleteShader
	~Shader();

	// Getters
	std::string getFilePath() const;
	GLuint getId() const;
	GLenum getType() const;
	std::string getCode() const;

	// Output, prints the id, the type, the file path and the code
	friend std::ostream& operator<<(std::ostream& os, const Shader& s);

private: 

	// Obtains the shader code from the filePath. Returns false if an error occurred while trying to read the file
	bool readShaderFromFile();

	// Compiles the shader and returns false if compilation failed
	bool compileShader();

	void _deleteObject() override;
};

#endif SHADER_H