#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class RenderSystem {
public:
    void initialize();
    void render(const glm::vec3& cameraPos, const glm::vec3& cameraFront, const glm::vec3& cameraUp);
    void cleanup();

private:
    void drawFloor();
    void drawWalls();
    void setupLighting();
    
    GLuint floorVAO = 0;
    GLuint floorVBO = 0;
};
