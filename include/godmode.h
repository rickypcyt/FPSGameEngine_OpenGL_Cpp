#ifndef GODMODE_H
#define GODMODE_H
#include <GLFW/glfw3.h>

#include <stdbool.h>

extern bool canFly;  // Variable global para controlar el vuelo

void toggleGodMode(bool active);  // Función para activar/desactivar el god mode

#endif // GODMODE_H
