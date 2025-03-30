#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../include/input/movement.h"
#include "../../include/core/globals.h"
#include "../../include/input/input.h"
#include "../../include/core/godmode.h"
#include "../../include/input/editor_input.h"
#include "../../include/ui/cursor.h"
#include <stdio.h>
#include <iostream>

// Movement constants
struct MovementConstants {
    static constexpr float JUMP_HEIGHT = 5.0f;
    static constexpr float GRAVITY = 20.0f;
    static constexpr float GROUND_LEVEL = 0.0f;
    static constexpr float AIR_CONTROL = 0.5f;
    static constexpr float ACCELERATION = 0.0f;
    static constexpr float DECELERATION = 0.0f;
    static constexpr float INITIAL_JUMP_VELOCITY = 8.0f;
};

// Global variables
MovementState moveState;
bool isDragging = false;
bool godMode = false;
float cubePos[3] = {0.0f, 0.0f, 0.0f};  // Posición del cubo en la escena

// Callback function definitions (outside namespace)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (EditorInput::isEditorMode) {
        EditorInput::handleKeyPress(window, key, scancode, action, mods);
        return;
    }
    
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_W: moveState.moveForward = true; break;
            case GLFW_KEY_S: moveState.moveBackward = true; break;
            case GLFW_KEY_A: moveState.moveLeft = true; break;
            case GLFW_KEY_D: moveState.moveRight = true; break;
            case GLFW_KEY_SPACE:
                if (godMode && canFly) {
                    moveState.moveUp = true;
                    moveState.moveDown = false;
                } else if (!godMode && characterPosY <= MovementConstants::GROUND_LEVEL) {
                    moveState.isJumping = true;
                    moveState.verticalVelocity = MovementConstants::INITIAL_JUMP_VELOCITY;
                }
                break;
            case GLFW_KEY_LEFT_SHIFT:
                if (godMode && canFly) {
                    moveState.moveDown = true;
                    moveState.moveUp = false;
                } else {
                    moveState.isRunning = true;
                }
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W: moveState.moveForward = false; break;
            case GLFW_KEY_S: moveState.moveBackward = false; break;
            case GLFW_KEY_A: moveState.moveLeft = false; break;
            case GLFW_KEY_D: moveState.moveRight = false; break;
            case GLFW_KEY_SPACE:
                if (godMode && canFly) {
                    moveState.moveUp = false;
                }
                break;
            case GLFW_KEY_LEFT_SHIFT:
                if (godMode && canFly) {
                    moveState.moveDown = false;
                } else {
                    moveState.isRunning = false;
                }
                break;
        }
    }

    // Activación/desactivación de God Mode
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        if (mods & GLFW_MOD_SHIFT) {
            godMode = !godMode;
            toggleGodMode(godMode);
            printf("God Mode: %s\n", godMode ? "Activado" : "Desactivado");
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (EditorInput::isEditorMode) {
        EditorInput::handleMouseMove(window, xpos, ypos);
        return;
    }
    
    handleCameraMouseMovement(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (EditorInput::isEditorMode) {
        EditorInput::handleMouseClick(window, button, action, mods);
        return;
    }
    
    // ... existing mouse button handling code ...
}

namespace Movement {
    void handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (EditorInput::isEditorMode) {
            EditorInput::handleKeyPress(window, key, scancode, action, mods);
        }
    }

    void handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
        if (EditorInput::isEditorMode) {
            EditorInput::handleMouseClick(window, button, action, mods);
        }
    }

    void updateJump(float deltaTime) {
        if (moveState.isJumping) {
            moveState.verticalVelocity -= MovementConstants::GRAVITY * deltaTime;
            characterPosY += moveState.verticalVelocity * deltaTime;

            // Land if below ground level
            if (characterPosY <= MovementConstants::GROUND_LEVEL) {
                characterPosY = MovementConstants::GROUND_LEVEL;
                moveState.verticalVelocity = 0.0f;
                moveState.isJumping = false;
            }
        }
    }

    void updateMovement(float deltaTime) {
        glm::vec3 moveDirection(0.0f);

        if (moveState.moveForward) moveDirection += cameraFront;
        if (moveState.moveBackward) moveDirection -= cameraFront;

        glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
        if (moveState.moveRight) moveDirection += right;
        if (moveState.moveLeft) moveDirection -= right;

        if (glm::length(moveDirection) > 0.0f) {
            moveDirection = glm::normalize(moveDirection);
            moveDirection.y = 0.0f;
        }

        if (godMode && canFly) {
            if (moveState.moveUp) moveDirection.y = 1.0f;
            if (moveState.moveDown) moveDirection.y = -1.0f;
        }

        float speed = moveSpeed;
        if (!godMode && moveState.isRunning) {
            speed *= 2.0f; // Velocidad de carrera
        }
        speed *= deltaTime;

        characterPosX += moveDirection.x * speed;
        characterPosY += moveDirection.y * speed;
        characterPosZ += moveDirection.z * speed;

        updateJump(deltaTime);
    }
}
