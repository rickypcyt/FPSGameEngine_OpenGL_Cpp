#include "../include/RenderSystem.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void RenderSystem::initialize() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    setupShaders();
    setupLighting();
    createGeometry();
}

void RenderSystem::render(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", cameraPos);

    drawFloor();
    drawWalls();
    drawObjects();
}

void RenderSystem::cleanup() {
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &wallVAO);
    glDeleteBuffers(1, &wallVBO);
    // Clean up other resources...
}

void RenderSystem::setupShaders() {
    shader.load("shaders/vertex.glsl", "shaders/fragment.glsl");
}

void RenderSystem::createGeometry() {
    createFloor();
    createWalls();
    // Create other geometries...
}

void RenderSystem::createFloor() {
    float vertices[] = {
        // Positions          // Normals           // Texture coords
        -10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
         10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,
        -10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,  0.0f,  10.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);

    glBindVertexArray(floorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void RenderSystem::createWalls() {
    // Similar to createFloor, but for walls
}

void RenderSystem::drawFloor() {
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    
    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void RenderSystem::drawWalls() {
    // Similar to drawFloor, but for walls
}

void RenderSystem::drawObjects() {
    // Draw other objects in your scene
}

void RenderSystem::setupLighting() {
    shader.use();
    shader.setVec3("light.position", glm::vec3(0.0f, 5.0f, 0.0f));
    shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
}
