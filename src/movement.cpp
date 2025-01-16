#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/movement.h"
#include "../include/globals.h"
#include "../include/input.h"
#include "../include/godmode.h"
#include <stdio.h>

bool isDragging = false;
bool godMode = false;
float cubePos[3] = {0.0f, 0.0f, 0.0f};  // Posición del cubo en la escena

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

// Movement state
struct MovementState {
    bool isJumping = false;
    bool isRunning = false;
    float verticalVelocity = 0.0f;
    
    // Estado de entrada
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
};

static MovementState moveState;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
