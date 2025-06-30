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
#include <algorithm>

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

glm::vec3 velocity(0.0f); // Global velocity for air movement

// Bunny hop, coyote time, jump buffer, and air strafe variables
bool isOnGround = true;
float coyoteTime = 0.0f;
float jumpBufferTime = 0.0f;
const float COYOTE_TIME_MAX = 0.12f;      // 120 ms
const float JUMP_BUFFER_MAX = 0.12f;      // 120 ms
const float MAX_AIR_SPEED = 8.0f;         // Limite de velocidad horizontal en el aire

// Parámetros para movimiento tipo Quake
const float AIR_ACCEL = 12.0f; // Aceleración en el aire
const float MAX_GROUND_SPEED = 8.0f;
const float AIR_FRICTION = 0.999f; // Muy baja fricción aérea

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
                } else if (isOnGround || coyoteTime > 0.0f) {
                    // --- TRANSFERENCIA DE VELOCIDAD HORIZONTAL AL SALTAR ---
                    glm::vec3 moveDir(0.0f);
                    if (moveState.moveForward) moveDir += cameraFront;
                    if (moveState.moveBackward) moveDir -= cameraFront;
                    glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
                    if (moveState.moveRight) moveDir += right;
                    if (moveState.moveLeft) moveDir -= right;
                    if (glm::length(moveDir) > 0.0f) {
                        moveDir = glm::normalize(moveDir);
                        moveDir.y = 0.0f;
                    }
                    float speed = moveSpeed;
                    if (!godMode && moveState.isRunning) speed *= 2.0f;
                    // Transfiere la velocidad horizontal actual al vector de aire
                    velocity.x = moveDir.x * speed;
                    velocity.z = moveDir.z * speed;
                    // Impulso vertical
                    moveState.isJumping = true;
                    moveState.verticalVelocity = MovementConstants::INITIAL_JUMP_VELOCITY;
                    isOnGround = false;
                    coyoteTime = 0.0f;
                    jumpBufferTime = 0.0f;
                } else {
                    jumpBufferTime = JUMP_BUFFER_MAX;
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
        // Coyote time y jump buffer
        if (!isOnGround) coyoteTime -= deltaTime;
        if (jumpBufferTime > 0.0f) jumpBufferTime -= deltaTime;
        if (moveState.isJumping) {
            moveState.verticalVelocity -= MovementConstants::GRAVITY * deltaTime;
            characterPosY += moveState.verticalVelocity * deltaTime;
            // Land if below ground level
            if (characterPosY <= MovementConstants::GROUND_LEVEL) {
                characterPosY = MovementConstants::GROUND_LEVEL;
                moveState.verticalVelocity = 0.0f;
                moveState.isJumping = false;
                isOnGround = true;
                coyoteTime = COYOTE_TIME_MAX;
            } else {
                isOnGround = false;
            }
        }
        // Bunny hop automático si hay jump buffer al aterrizar
        if (isOnGround && jumpBufferTime > 0.0f) {
            moveState.isJumping = true;
            moveState.verticalVelocity = MovementConstants::INITIAL_JUMP_VELOCITY;
            isOnGround = false;
            jumpBufferTime = 0.0f;
            coyoteTime = 0.0f;
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
        if (!godMode && moveState.isRunning) speed *= 2.0f;
        speed *= deltaTime;
        // --- MOVIMIENTO EN EL AIRE TIPO QUAKE ---
        if (!isOnGround) {
            // Air strafe: proyección y aceleración
            if (glm::length(moveDirection) > 0.0f) {
                glm::vec3 wishdir = glm::normalize(moveDirection);
                float wishspeed = MAX_AIR_SPEED;
                float currentspeed = glm::dot(velocity, wishdir);
                float addspeed = wishspeed - currentspeed;
                if (addspeed > 0) {
                    float accelspeed = std::min(addspeed, AIR_ACCEL * deltaTime);
                    velocity += wishdir * accelspeed;
                }
            }
            // Limita la velocidad horizontal máxima en el aire
            glm::vec3 horizVel = velocity; horizVel.y = 0.0f;
            float horizSpeed = glm::length(horizVel);
            if (horizSpeed > MAX_AIR_SPEED) {
                horizVel = glm::normalize(horizVel) * MAX_AIR_SPEED;
                velocity.x = horizVel.x;
                velocity.z = horizVel.z;
            }
            // Aplica la velocidad
            characterPosX += velocity.x * deltaTime;
            characterPosY += velocity.y * deltaTime;
            characterPosZ += velocity.z * deltaTime;
            // Fricción aérea mínima
            velocity.x *= AIR_FRICTION;
            velocity.z *= AIR_FRICTION;
        } else {
            // Movimiento en el suelo tipo Quake
            if (glm::length(moveDirection) > 0.0f) {
                glm::vec3 wishdir = glm::normalize(moveDirection);
                float wishspeed = MAX_GROUND_SPEED;
                float currentspeed = glm::dot(velocity, wishdir);
                float addspeed = wishspeed - currentspeed;
                if (addspeed > 0) {
                    float accelspeed = std::min(addspeed, AIR_ACCEL * 2.0f * deltaTime); // Más aceleración en el suelo
                    velocity += wishdir * accelspeed;
                }
            } else {
                // Fricción en el suelo
                velocity.x *= 0.8f;
                velocity.z *= 0.8f;
            }
            // Limita la velocidad horizontal máxima en el suelo
            glm::vec3 horizVel = velocity; horizVel.y = 0.0f;
            float horizSpeed = glm::length(horizVel);
            if (horizSpeed > MAX_GROUND_SPEED) {
                horizVel = glm::normalize(horizVel) * MAX_GROUND_SPEED;
                velocity.x = horizVel.x;
                velocity.z = horizVel.z;
            }
            // Aplica la velocidad
            characterPosX += velocity.x * deltaTime;
            characterPosY += velocity.y * deltaTime;
            characterPosZ += velocity.z * deltaTime;
        }
        updateJump(deltaTime);
    }
}
