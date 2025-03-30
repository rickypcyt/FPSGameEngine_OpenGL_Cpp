#ifndef GLOBALS_H
#define GLOBALS_H

#include <glm/glm.hpp>

// Define global variables
extern float moveSpeed;      // Movement speed
extern float moveSpeedX;     // Forward/Backward movement speed
extern float moveSpeedY;     // Left/Right movement speed

extern float characterPosX;  // Character position X
extern float characterPosY;  // Character position Y
extern float characterPosZ;  // Character position Z
extern float deltaTime;      // Time between frames

const int WIDTH = 1920;      // Window width
const int HEIGHT = 1080;     // Window height

extern float pitch;          // Camera pitch
extern float yaw;            // Camera yaw

extern glm::vec3 cameraPos;  // Camera position
extern glm::vec3 cameraFront; // Direction the camera faces
extern glm::vec3 cameraUp;    // Upward direction for the camera

extern float sensitivity;      // Mouse sensitivity

// Additional global variables for enhanced gameplay
extern bool isJumping;        // Jumping state
extern float jumpHeight;      // Jump height
extern float gravity;         // Gravity force

#endif // GLOBALS_H
