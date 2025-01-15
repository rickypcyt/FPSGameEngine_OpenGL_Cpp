#include "../include/godmode.h"
#include "../include/input.h"
#include <stdio.h>

bool canFly = false;

// Función para activar o desactivar el god mode
void toggleGodMode(bool active) {
    if (active) {
        printf("¡God Mode activado!\n");
        // Aquí activamos el vuelo
        canFly = true;
        // Puedes agregar más características aquí, como invulnerabilidad, mayor velocidad, etc.
    } else {
        printf("¡God Mode desactivado!\n");
        // Desactivamos el vuelo
        canFly = false;
        // Restablece las características del god mode
    }
}

void mouse_button_callback_godmode(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);  // Guarda la posición inicial del cursor cuando se presiona el botón
        } else if (action == GLFW_RELEASE) {
            isDragging = false;  // Deja de arrastrar cuando el botón se suelta
        }
    }
}

// Esta es la función para mover el cubo, puedes cambiarla según tus necesidades
void moveObject(float deltaX, float deltaY) {
    cubePos[0] += deltaX * 0.01f;
    cubePos[1] -= deltaY * 0.01f;
    printf("Nuevo cubo posición: X = %f, Y = %f\n", cubePos[0], cubePos[1]);
}

void mouse_callback_godmode(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        double deltaX = xpos - lastX;
        double deltaY = ypos - lastY;

        if (godMode) {
            // Mueve el cubo cuando el god mode está activado
            moveObject(deltaX, deltaY);
        }

        // Actualiza la posición del cursor para la siguiente iteración
        lastX = xpos;
        lastY = ypos;
    }
}