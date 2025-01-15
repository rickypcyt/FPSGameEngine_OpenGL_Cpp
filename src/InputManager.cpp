#include "../include/InputManager.h"

InputManager* InputManager::instance = nullptr;

void InputManager::initialize(GLFWwindow* window) {
    this->window = window;
    instance = this;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    lastX = width / 2.0f;
    lastY = height / 2.0f;
}

void InputManager::update() {
    // Any necessary updates for input
}

bool InputManager::isKeyPressed(int key) const {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!instance) return;

    if (instance->firstMouse) {
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstMouse = false;
    }

    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos; // Invert y offset

    instance->lastX = xpos;
    instance->lastY = ypos;

    xoffset *= instance->sensitivity;
    yoffset *= instance->sensitivity;

    instance->yaw += xoffset;
    instance->pitch += yoffset;

    // Constrain pitch
    if (instance->pitch > 89.0f) instance->pitch = 89.0f;
    if (instance->pitch < -89.0f) instance->pitch = -89.0f;

    // Update camera front direction
    glm::vec3 front;
    front.x = cos(glm::radians(instance->yaw)) * cos(glm::radians(instance->pitch));
    front.y = sin(glm::radians(instance->pitch));
    front.z = sin(glm::radians(instance->yaw)) * cos(glm::radians(instance->pitch));
    instance->cameraFront = glm::normalize(front);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }
}
