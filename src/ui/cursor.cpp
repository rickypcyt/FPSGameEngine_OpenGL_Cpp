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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    // Calcular el offset del mouse
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    
    // Actualizar la última posición del mouse
    lastX = xpos;
    lastY = ypos;

    // Ajustar la sensibilidad del mouse
    const float sensitivity = 0.1f; // Reducida para un control más suave

    // Aplicar la sensibilidad
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Actualizar yaw y pitch
    yaw += xoffset;
    pitch += yoffset;

    // Limitar el pitch para evitar que la cámara se voltee
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    // Actualizar la dirección de la cámara
    updateCameraDirection();
}
