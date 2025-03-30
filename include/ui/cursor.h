#pragma once
#include <GLFW/glfw3.h>

// Function declarations
void handleCameraKeys(GLFWwindow* window, int key, int action);
void centerCursor(GLFWwindow* window);
void updateCamera(float xpos, float ypos);
void updateCameraDirection();
void handleCameraMouseMovement(double xpos, double ypos);


