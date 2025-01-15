#pragma once
#include <GLFW/glfw3.h>

// Function declarations
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void handleCameraKeys(GLFWwindow* window, int key, int action);
void centerCursor(GLFWwindow* window);
void updateCamera(float xpos, float ypos);


