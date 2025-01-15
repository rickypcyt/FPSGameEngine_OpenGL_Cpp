#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/cursor.h"
#include "../include/globals.h"

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// Function to update the camera direction
void updateCameraDirection() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Check if first mouse input to initialize lastX and lastY
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate offsets
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // Inverted y-axis for proper movement
    lastX = xpos;
    lastY = ypos;

    // Adjust sensitivity
    const float sensitivity = 0.1f; // Decrease sensitivity if needed
    //const float sensitivity = 0.02f; // Try a lower sensitivity
    const float smoothingFactor = 0.1f; // Adjust smoothing factor as needed

    // Apply smoothing
    xoffset = (xoffset * smoothingFactor) + (lastX - xpos) * (1.0f - smoothingFactor);
    yoffset = (yoffset * smoothingFactor) + (lastY - ypos) * (1.0f - smoothingFactor);

    // Update yaw and pitch
    yaw += xoffset * sensitivity; // Adjust sensitivity here
    pitch += yoffset * sensitivity;

    // Clamp pitch to prevent inverted view
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    // Update camera direction based on new yaw and pitch
    updateCameraDirection();
}
