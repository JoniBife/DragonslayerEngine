#include <fstream>
#include <sstream>
#include "Shader.h"
#include "../utils/OpenGLUtils.h"

// Default constructor
Shader::Shader() { /*EMPTY*/ } 

// Receives the shader file path and compiles the shader
Shader::Shader(const GLenum shaderType, const std::string& filePath) : type(shaderType), filePath(filePath) {

    if (!readShaderFromFile()) { 
        exit(EXIT_FAILURE);
    }

    if (!compileShader()) {
        exit(EXIT_FAILURE);
    }
}
// Deletes the shader using glDeleteShader
Shader::~Shader() {
    GL_CALL(glDeleteShader(id));
}

// Getters
std::string Shader::getFilePath() const {
    return filePath;
}
GLuint Shader::getId() const {
    return id;
}
GLenum Shader::getType() const {
    return type;
}
std::string Shader::getCode() const {
    return code;
}

// Output, prints the id, the type, the file path and the code
std::ostream& operator<<(std::ostream& os, const Shader& s) {
    // TODO
    return os;
}

// Obtains the shader code from the filePath. Returns false if an error occurred while trying to read the file
bool Shader::readShaderFromFile() {

    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        code = shaderStream.str();
        return true;
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Filestream ERROR [ Shader file not successfully read | Exception: " << e.what() << " ]" << std::endl;
        // streams must always be closed
        if (shaderFile.is_open()) {
            shaderFile.close();
        }
        return false;
    }
}

std::string getShaderTypeName(GLenum shaderType) {
    switch (shaderType) {
    case GL_VERTEX_SHADER: return "Vertex shader";
    case GL_FRAGMENT_SHADER: return "Fragment shader";
    case GL_GEOMETRY_SHADER: return "Geometry shader";
    default: return "no shader type";
    };
    return "";
}
// Compiles the shader and returns false if compilation failed
bool Shader::compileShader() {
    GL_CALL(id = glCreateShader(type));
    const char* src = code.c_str();
    GL_CALL(glShaderSource(id, 1, &src, NULL));
    GL_CALL(glCompileShader(id));

    GLint compileStatus;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE)
    {
        int logLength;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength));
        char* infoLog = new char[logLength]();
        GL_CALL(glGetShaderInfoLog(id, logLength, &logLength, infoLog));

        std::cout << "Shader compilation ERROR [ Failed to compile a " 
            << getShaderTypeName(type) << " | log: " << infoLog << " ]" << std::endl;

        delete[] infoLog;
        GL_CALL(glDeleteShader(id));
        return false;
    }
    return true;
}

