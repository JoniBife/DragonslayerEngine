#include <assert.h>
#include "ShaderProgram.h"
#include "../utils/OpenGLUtils.h"

#define VERTICES 0
#define NORMALS 1
#define COLORS 2
#define TEXTCOORDS 3
#define TANGENTS 4

// In the future we should add other constructors to support other types of shaders
ShaderProgram::ShaderProgram(Shader& vertexShader, Shader& fragmentShader)
{
	GL_CALL(id = glCreateProgram());

    // Attaching shaders to program
    GL_CALL(glAttachShader(id, vertexShader.getId()));
    GL_CALL(glAttachShader(id, fragmentShader.getId()));
    
    // This step is unnecessary if you use the location specifier in your shader
    // e.g. layout (location = 0) in vec3 in_Position;
    // Even if the shader does not contain one of these attributes its alright
    // OpenGL Docs: " It is also permissible to bind a generic attribute index to an attribute variable name that is never used in a vertex shader."
    GL_CALL(glBindAttribLocation(id, VERTICES, "inPosition"));
    GL_CALL(glBindAttribLocation(id, NORMALS, "inNormal"));
    GL_CALL(glBindAttribLocation(id, COLORS, "inColor"));
    GL_CALL(glBindAttribLocation(id, TEXTCOORDS, "inTextCoord"));
    GL_CALL(glBindAttribLocation(id, TANGENTS, "inTangent"));

	GL_CALL(glLinkProgram(id));

    GLint programLinked;
    GL_CALL(glGetProgramiv(id, GL_LINK_STATUS, &programLinked));

    if (programLinked != GL_TRUE) {
        int logLength;
        GL_CALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength));
        char* infoLog = new char[logLength]();
        GL_CALL(glGetProgramInfoLog(id, logLength, &logLength, infoLog));
        std::cout << "Program linking ERROR [ " << infoLog << " ]" << std::endl;
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        GL_CALL(glDeleteProgram(id));
        exit(EXIT_FAILURE);
    }

    // Program has been created and successfully linked so we detach the shaders
    GL_CALL(glDetachShader(id, vertexShader.getId()));
    GL_CALL(glDetachShader(id, fragmentShader.getId()));
}

ShaderProgram::ShaderProgram(Shader& vertexShader, Shader& geometryShader, Shader& fragmentShader)
{
    GL_CALL(id = glCreateProgram());

    // Attaching shaders to program
    GL_CALL(glAttachShader(id, vertexShader.getId()));
    GL_CALL(glAttachShader(id, geometryShader.getId()));
    GL_CALL(glAttachShader(id, fragmentShader.getId()));

    // This step is unnecessary if you use the location specifier in your shader
    // e.g. layout (location = 0) in vec3 in_Position;
    // Even if the shader does not contain one of these attributes its alright
    // OpenGL Docs: " It is also permissible to bind a generic attribute index to an attribute variable name that is never used in a vertex shader."
    GL_CALL(glBindAttribLocation(id, VERTICES, "inPosition"));
    GL_CALL(glBindAttribLocation(id, NORMALS, "inNormal"));
    GL_CALL(glBindAttribLocation(id, COLORS, "inColor"));
    GL_CALL(glBindAttribLocation(id, TEXTCOORDS, "inTextCoord"));
    GL_CALL(glBindAttribLocation(id, TANGENTS, "inTangent"));

    GL_CALL(glLinkProgram(id));

    GLint programLinked;
    GL_CALL(glGetProgramiv(id, GL_LINK_STATUS, &programLinked));

    if (programLinked != GL_TRUE) {
        int logLength;
        GL_CALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength));
        char* infoLog = new char[logLength]();
        GL_CALL(glGetProgramInfoLog(id, logLength, &logLength, infoLog));
        std::cout << "Program linking ERROR [ " << infoLog << " ]" << std::endl;
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        GL_CALL(glDeleteProgram(id));
        exit(EXIT_FAILURE);
    }

    // Program has been created and successfully linked so we detach the shaders
    GL_CALL(glDetachShader(id, vertexShader.getId()));
    GL_CALL(glDetachShader(id, geometryShader.getId()));
    GL_CALL(glDetachShader(id, fragmentShader.getId()));
}

ShaderProgram::~ShaderProgram() {
    GL_CALL(glDeleteProgram(id));
}

void ShaderProgram::use() {
    GL_CALL(glUseProgram(id));
}
void ShaderProgram::stopUsing() {
    GL_CALL(glUseProgram(0));
}

GLuint ShaderProgram::getProgramId() const {
    return id;
}

// All theses methods se the value of a specific uniform
void ShaderProgram::setUniform(const GLint location, const GLint value) {
    assert(location >= 0); // execution should stop if location is < 0
    GL_CALL(glUniform1i(location, value));
}
void ShaderProgram::setUniform(const GLint location, const GLuint value) {
    assert(location >= 0); // execution should stop if location is < 0
    GL_CALL(glUniform1ui(location, value));
}
void ShaderProgram::setUniform(const GLint location, const GLfloat value) {
    assert(location >= 0); // execution should stop if location is < 0
    GL_CALL(glUniform1f(location, value));
}
void ShaderProgram::setUniform(const GLint location, const Vec2& value) {
    assert(location >= 0); // execution should stop if location is < 0
    float array[2];
    value.toOpenGLFormat(array);
    GL_CALL(glUniform2fv(location, 1, array));
}
void ShaderProgram::setUniform(const GLint location, const Vec3& value) {
    assert(location >= 0); // execution should stop if location is < 0
    float array[3];
    value.toOpenGLFormat(array);
    GL_CALL(glUniform3fv(location, 1, array));
}
void ShaderProgram::setUniform(const GLint location, const Vec4& value) {
    assert(location >= 0); // execution should stop if location is < 0
    float array[4];
    value.toOpenGLFormat(array);
    GL_CALL(glUniform4fv(location, 1, array));
}
void ShaderProgram::setUniform(const GLint location, const Mat2& value) {
    assert(location >= 0); // execution should stop if location is < 0
    float array[4];
    value.toOpenGLFormat(array);
    GL_CALL(glUniformMatrix2fv(location, 1, GL_FALSE, array));
}
void ShaderProgram::setUniform(const GLint location, const Mat3& value) {
    assert(location >= 0); // execution should stop if location is < 0
    float array[9];
    value.toOpenGLFormat(array);
    GL_CALL(glUniformMatrix3fv(location, 1, GL_FALSE, array));
}
void ShaderProgram::setUniform(const GLint location, const Mat4& value) {
    assert(location >= 0); // execution should stop if location is < 0
    float array[16];
    value.toOpenGLFormat(array);
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, array));
}

// Binds a uniform block
void ShaderProgram::bindUniformBlock(const GLuint index, const GLuint bindingPoint) {
    GL_CALL(glUniformBlockBinding(id, index, bindingPoint));
}

// Returns the index of the uniform block with name
GLuint ShaderProgram::getUniformBlockIndex(const GLchar* name) {
    GL_CALL(GLuint index = glGetUniformBlockIndex(id, name));
    assert(index != GL_INVALID_INDEX);
    return index;
}

// Used to obtain the location of uniforms, this way we avoid doing this in every setUniform
GLint ShaderProgram::getUniformLocation(const GLchar* name) const {
    GL_CALL(GLint location =  glGetUniformLocation(id, name));
    //assert(location >= 0); // execution should stop if location is < 0
    return location;
}

// Output, prints the id
std::ostream& operator<<(std::ostream& os, const ShaderProgram& sp) {
    // TODO
    return os;
}