#include "../../include/graphics/modern_renderer.h"
#include <iostream>

ModernRenderer::ModernRenderer() 
    : cameraPos(0.0f, 0.0f, 3.0f)
    , cameraFront(0.0f, 0.0f, -1.0f)
    , cameraUp(0.0f, 1.0f, 0.0f)
    , yaw(-90.0f)
    , pitch(0.0f)
    , lightPos(1.2f, 1.0f, 2.0f)
    , lightColor(1.0f, 1.0f, 1.0f) {
    
    // Initialize matrices
    projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
}

ModernRenderer::~ModernRenderer() {
}

bool ModernRenderer::initialize() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Setup shaders
    setupShaders();
    
    // Setup meshes
    setupMeshes();
    
    // Set default projection
    setProjection(45.0f, 800.0f/600.0f, 0.1f, 100.0f);
    
    // Update view matrix
    updateViewMatrix();
    
    return true;
}

void ModernRenderer::setupShaders() {
    shader = std::make_unique<Shader>();
    
    // Load shaders from files
    if (!shader->loadFromFiles("src/shaders/modern_vertex.glsl", "src/shaders/modern_fragment.glsl")) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return;
    }
    
    shader->use();
    
    // Set default uniforms
    shader->setVec3("lightPos", lightPos);
    shader->setVec3("lightColor", lightColor);
    shader->setBool("useLighting", true);
}

void ModernRenderer::setupMeshes() {
    // Create default meshes
    cubeMesh = std::make_unique<Mesh>(Mesh::createCube(1.0f));
    planeMesh = std::make_unique<Mesh>(Mesh::createPlane(1.0f, 1.0f));
    sphereMesh = std::make_unique<Mesh>(Mesh::createSphere(1.0f, 32));
}

void ModernRenderer::beginFrame() {
    shader->use();
    
    // Update view matrix
    updateViewMatrix();
    
    // Set matrices in shader
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    
    // Set lighting uniforms
    shader->setVec3("lightPos", lightPos);
    shader->setVec3("lightColor", lightColor);
    shader->setVec3("viewPos", cameraPos);
}

void ModernRenderer::endFrame() {
    // Unbind shader
    glUseProgram(0);
}

void ModernRenderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ModernRenderer::drawCube(const glm::vec3& position, const glm::vec3& scale) {
    if (!cubeMesh) return;
    
    // Set model matrix
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    
    shader->setMat4("model", model);
    
    // Draw the cube
    cubeMesh->draw();
}

void ModernRenderer::drawPlane(const glm::vec3& position, const glm::vec2& scale) {
    if (!planeMesh) return;
    
    // Set model matrix
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale.x, 1.0f, scale.y));
    
    shader->setMat4("model", model);
    
    // Draw the plane
    planeMesh->draw();
}

void ModernRenderer::drawSphere(const glm::vec3& position, float radius) {
    if (!sphereMesh) return;
    
    // Set model matrix
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));
    
    shader->setMat4("model", model);
    
    // Draw the sphere
    sphereMesh->draw();
}

void ModernRenderer::drawMesh(const Mesh& mesh, const glm::vec3& position, const glm::vec3& scale) {
    // Set model matrix
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    
    shader->setMat4("model", model);
    
    // Draw the mesh
    mesh.draw();
}

void ModernRenderer::setCameraPosition(const glm::vec3& pos) {
    cameraPos = pos;
    updateViewMatrix();
}

void ModernRenderer::setCameraDirection(float newYaw, float newPitch) {
    yaw = newYaw;
    pitch = newPitch;
    
    // Constrain pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    // Calculate front vector
    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(cameraFront);
    
    updateViewMatrix();
}

void ModernRenderer::updateCamera() {
    updateViewMatrix();
}

void ModernRenderer::updateViewMatrix() {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void ModernRenderer::setLightPosition(const glm::vec3& pos) {
    lightPos = pos;
}

void ModernRenderer::setLightColor(const glm::vec3& color) {
    lightColor = color;
}

void ModernRenderer::enableLighting(bool enable) {
    shader->setBool("useLighting", enable);
}

void ModernRenderer::setProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void ModernRenderer::setViewMatrix(const glm::mat4& newView) {
    view = newView;
}

void ModernRenderer::setModelMatrix(const glm::mat4& newModel) {
    model = newModel;
} 