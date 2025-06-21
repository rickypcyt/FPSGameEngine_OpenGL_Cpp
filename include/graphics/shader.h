#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    
    std::string vertexSource;
    std::string fragmentSource;
    
    bool compileShader(GLuint shaderID, const std::string& source);
    bool linkProgram();
    void checkCompileErrors(GLuint shader, std::string type);

public:
    Shader();
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    void use();
    GLuint getProgramID() const { return programID; }
    
    // Utility uniform functions
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);
    
    // Load shaders from files
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromStrings(const std::string& vertexSource, const std::string& fragmentSource);
};

#endif // SHADER_H 