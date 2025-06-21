#include "../../include/graphics/shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : programID(0), vertexShaderID(0), fragmentShaderID(0) {
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) 
    : programID(0), vertexShaderID(0), fragmentShaderID(0) {
    loadFromFiles(vertexPath, fragmentPath);
}

Shader::~Shader() {
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

void Shader::use() {
    glUseProgram(programID);
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read vertex shader
    std::ifstream vertexFile(vertexPath);
    if (!vertexFile.is_open()) {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND: " << vertexPath << std::endl;
        return false;
    }
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    vertexSource = vertexStream.str();
    vertexFile.close();

    // Read fragment shader
    std::ifstream fragmentFile(fragmentPath);
    if (!fragmentFile.is_open()) {
        std::cerr << "ERROR::SHADER::FRAGMENT::FILE_NOT_FOUND: " << fragmentPath << std::endl;
        return false;
    }
    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    fragmentSource = fragmentStream.str();
    fragmentFile.close();

    return loadFromStrings(vertexSource, fragmentSource);
}

bool Shader::loadFromStrings(const std::string& vertexSource, const std::string& fragmentSource) {
    this->vertexSource = vertexSource;
    this->fragmentSource = fragmentSource;

    // Create shaders
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile shaders
    if (!compileShader(vertexShaderID, vertexSource)) {
        return false;
    }
    if (!compileShader(fragmentShaderID, fragmentSource)) {
        return false;
    }

    // Create program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    // Link program
    if (!linkProgram()) {
        return false;
    }

    // Clean up shaders
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return true;
}

bool Shader::compileShader(GLuint shaderID, const std::string& source) {
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        checkCompileErrors(shaderID, "SHADER");
        return false;
    }
    return true;
}

bool Shader::linkProgram() {
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        checkCompileErrors(programID, "PROGRAM");
        return false;
    }
    return true;
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                      << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                      << infoLog << std::endl;
        }
    }
}

// Uniform setters
void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
} 