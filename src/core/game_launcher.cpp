#include "../../include/core/menu.h"
#include <iostream>
#include <GL/freeglut.h>

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Solo una vez por proceso
    // Show the main menu
    Menu::showMenu();
    
    return 0;
} 