#ifndef MODERN_RENDERER_H
#define MODERN_RENDERER_H

#include "shader.h"
#include "mesh.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ModernRenderer {
private:
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> cubeMesh;
    std::unique_ptr<Mesh> planeMesh;
    std::unique_ptr<Mesh> sphereMesh;
    
    // Camera properties
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float yaw;
    float pitch;
    
    // Lighting
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    
    // Matrices
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
    
    void updateViewMatrix();
    void setupShaders();
    void setupMeshes();

public:
    ModernRenderer();
    ~ModernRenderer();
    
    // Initialization
    bool initialize();
    
    // Camera control
    void setCameraPosition(const glm::vec3& pos);
    void setCameraDirection(float yaw, float pitch);
    void updateCamera();
    
    // Rendering
    void beginFrame();
    void endFrame();
    void clear();
    
    // Drawing functions
    void drawCube(const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.0f));
    void drawPlane(const glm::vec3& position, const glm::vec2& scale = glm::vec2(1.0f));
    void drawSphere(const glm::vec3& position, float radius = 1.0f);
    void drawMesh(const Mesh& mesh, const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.0f));
    
    // Lighting
    void setLightPosition(const glm::vec3& pos);
    void setLightColor(const glm::vec3& color);
    void enableLighting(bool enable);
    
    // Matrix setters
    void setProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setViewMatrix(const glm::mat4& view);
    void setModelMatrix(const glm::mat4& model);
    
    // Getters
    const glm::mat4& getProjectionMatrix() const { return projection; }
    const glm::mat4& getViewMatrix() const { return view; }
    const glm::mat4& getModelMatrix() const { return model; }
    const glm::vec3& getCameraPosition() const { return cameraPos; }
};

#endif // MODERN_RENDERER_H 