#include "../include/globals.h"

// Define global variables
float moveSpeed = 5.0f;      // Movement speed
float moveSpeedX = 0.0f;     // Forward/Backward movement speed
float moveSpeedY = 0.0f;     // Left/Right movement speed

float characterPosX = 0.0f;
float characterPosY = 0.0f;
float characterPosZ = 5.0f;
float deltaTime = 0.0f;

float pitch = 0.0f;           // Define pitch
float yaw = -90.0f;           // Define yaw

glm::vec3 cameraPos(0.0f, 1.0f, 5.0f);    // Adjusted camera position for better starting view
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);  // Initial direction the camera faces
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);      // Upward direction for the camera

float sensitivity = 0.5f;       // Mouse sensitivity

// Additional global variables for enhanced gameplay
bool isJumping = false;         // Jumping state initialized to false
float jumpHeight = 5.0f;        // Default jump height (can be adjusted)
float gravity = -9.81f;         // Gravity force (can be adjusted)
