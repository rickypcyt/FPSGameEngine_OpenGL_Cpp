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
    static constexpr float GRAVITY = 20.0f;         // Adjusted gravity for better feel
    static constexpr float GROUND_LEVEL = 0.0f;
    static constexpr float AIR_CONTROL = 0.5f;       // Reduced air control for more realistic jumps
    static constexpr float ACCELERATION = 0.0f;      // Instantaneous acceleration
    static constexpr float DECELERATION = 0.0f;      // Instantaneous deceleration
    static constexpr float INITIAL_JUMP_VELOCITY = 8.0f; // Adjusted for more height
};

// Movement state
// Estructura de estado de movimiento
struct MovementState {
    bool isJumping = false;
    float verticalVelocity = 0.0f;
    
    // Estado de entrada
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;  // Para movimiento hacia arriba (flotación)
    bool moveDown = false; // Para movimiento hacia abajo (descenso)
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
                    // Si estamos en God Mode y podemos volar, flotamos hacia arriba
                    moveState.moveUp = true;
                    moveState.moveDown = false; // Aseguramos que no baje si está flotando
                } else if (!godMode && characterPosY <= MovementConstants::GROUND_LEVEL) {
                    // Salto normal si no estamos en God Mode
                    moveState.isJumping = true;
                    moveState.verticalVelocity = MovementConstants::INITIAL_JUMP_VELOCITY;
                }
                break;
            case GLFW_KEY_LEFT_SHIFT:  // Asegurándonos de que Shift sea reconocido correctamente
                if (godMode && canFly) {
                    // Si estamos en God Mode y podemos volar, descendemos
                    moveState.moveDown = true;
                    moveState.moveUp = false;  // Aseguramos que no suba si está bajando
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
                    // Dejar de flotar cuando se suelta la tecla Space
                    moveState.moveUp = false;
                }
                break;
            case GLFW_KEY_LEFT_SHIFT:  // Asegurándonos de que Shift sea reconocido correctamente
                if (godMode && canFly) {
                    // Dejar de bajar cuando se suelta Shift
                    moveState.moveDown = false;
                }
                break;
        }
    }

    // Activación/desactivación de God Mode
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        if (mods & GLFW_MOD_SHIFT) {  // Detecta si Shift está presionado junto con G
            godMode = !godMode;  // Activa o desactiva el god mode
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
    // Calculamos la dirección del movimiento
    glm::vec3 moveDirection(0.0f);

    // Movimiento hacia adelante/atrás
    if (moveState.moveForward) moveDirection += cameraFront;
    if (moveState.moveBackward) moveDirection -= cameraFront;

    // Movimiento lateral (izquierda/derecha)
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    if (moveState.moveRight) moveDirection += right;
    if (moveState.moveLeft) moveDirection -= right;

    // Normalizamos la dirección de movimiento
    if (glm::length(moveDirection) > 0.0f) {
        moveDirection = glm::normalize(moveDirection);
        moveDirection.y = 0.0f; // Prevenir el movimiento vertical cuando no estamos flotando o descendiendo
    }

    // Si estamos en el god mode y podemos volar, habilitamos el movimiento vertical
    if (godMode && canFly) {
        if (moveState.moveUp) moveDirection.y = 1.0f;  // Mover hacia arriba (flotar)
        if (moveState.moveDown) moveDirection.y = -1.0f;  // Mover hacia abajo (descender)
    }

    // Aplicar movimiento con velocidad y deltaTime
    float speed = moveSpeed * deltaTime; // Aseguramos que el movimiento no dependa de la tasa de refresco
    characterPosX += moveDirection.x * speed;
    characterPosY += moveDirection.y * speed;  // Actualizar la posición Y
    characterPosZ += moveDirection.z * speed;

    // Actualizamos la lógica de salto
    updateJump(deltaTime);
}
