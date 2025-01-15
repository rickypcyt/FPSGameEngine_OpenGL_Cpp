#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <GLFW/glfw3.h>

// External variables for movement speeds
extern float moveSpeed;       // Movement speed
extern float moveSpeedX;      // Forward/Backward movement speed
extern float moveSpeedY;      // Left/Right movement speed

// Jump variables
extern bool isJumping;        // Track whether the character is currently jumping
extern float jumpHeight;      // Maximum height of the jump
extern float verticalVelocity; // Current vertical velocity

// Window dimensions (defined elsewhere)
// Character position
extern float characterPosX;   // Character's X position
extern float characterPosY;   // Character's Y position
extern float characterPosZ;   // Character's Z position

// Time-related variable
extern float deltaTime;       // Time since last frame

// Function declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void updateMovement(float deltaTime); // Ensure deltaTime is passed
void updateCamera();                   // Assuming this function exists

#endif // MOVEMENT_H
