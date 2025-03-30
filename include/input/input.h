#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

// Variables globales para el estado del mouse y el modo "god mode"
extern bool isDragging;
extern double lastX, lastY;
extern bool godMode;
extern float cubePos[3];  // Posición del cubo en la escena

// Declaraciones de las funciones de callback
void mouse_callback_godmode(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback_godmode(GLFWwindow* window, int button, int action, int mods);
void key_callback_global(GLFWwindow* window, int key, int scancode, int action, int mods);

// Inicializa los callbacks de entrada
void initializeInput(GLFWwindow* window);

#endif // INPUT_H
