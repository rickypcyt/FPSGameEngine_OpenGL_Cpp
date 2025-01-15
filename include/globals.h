#ifndef GLOBALS_H
#define GLOBALS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

extern float characterPosX;
extern float characterPosY;
extern float characterPosZ;
extern float deltaTime;
extern const int WIDTH;
extern const int HEIGHT;
extern GLFWwindow* window;


extern float pitch;  // Declare pitch as extern
extern float yaw;    // Declare yaw as extern
extern glm::vec3 cameraPos;
extern glm::vec3 cameraUp;
extern glm::vec3 cameraFront;
extern float sensitivity;

#endif // GLOBALS_H
