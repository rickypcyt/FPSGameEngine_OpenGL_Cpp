#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../include/ui/cursor.h"
#include "../../include/core/globals.h"

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// Function to update the camera direction
void updateCameraDirection() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// Function to handle mouse movement for camera control
void handleCameraMouseMovement(double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // Inverted y-axis for proper movement
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.15f; // Increased sensitivity for quicker response

    // Apply sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Update yaw and pitch
    yaw += xoffset; 
    pitch += yoffset;

    // Clamp pitch to prevent inverted view
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    // Update camera direction based on new yaw and pitch
    updateCameraDirection();
}
