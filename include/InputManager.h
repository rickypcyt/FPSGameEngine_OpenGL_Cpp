#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager {
public:
    void initialize(GLFWwindow* window);
    void update();
    
    const glm::vec3& getCameraFront() const { return cameraFront; }
    const glm::vec3& getCameraUp() const { return cameraUp; }
    bool isKeyPressed(int key) const;
    
    float getPitch() const { return pitch; }
    float getYaw() const { return yaw; }
    
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    static InputManager* instance;
    GLFWwindow* window = nullptr;
    
    glm::vec3 cameraFront{0.0f, 0.0f, -1.0f};
    glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
    
    float pitch = 0.0f;
    float yaw = -90.0f;
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = true;
    float sensitivity = 0.1f;
};
