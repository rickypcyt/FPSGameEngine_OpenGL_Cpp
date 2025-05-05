#include <GL/gl.h>
#include <GL/glu.h>
#include "../../include/graphics/renderer.h"
#include "../../include/graphics/lights.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "../../include/models/character.h"

Character* playerCharacter = nullptr;

// Función principal de dibujo
void drawScene() {
    drawCheckerboardFloor(20.0f, 1.0f); // Dibujar el piso de ajedrez
glTranslatef(0.0f, 2.5f, 0.0f); // ← Mover el cubo hacia arriba (la mitad de su tamaño)
    drawCube(2.5);                     // Dibujar el cubo que rodea al jugador
        glPopMatrix();

    if (playerCharacter) {
        glPushMatrix();
        glTranslatef(5.0f, 0.0f, -5.0f); // Posicionar el personaje
        glScalef(0.1f, 0.1f, 0.1f);      // Escalar si está muy grande
        playerCharacter->draw();
        glPopMatrix();
    }
}

void initializeOpenGL() {
    glEnable(GL_DEPTH_TEST); // Habilitar pruebas de profundidad para un renderizado adecuado
    setupLighting();         // Configurar iluminación si se desea
        playerCharacter = new Character();
    if (!playerCharacter->load("../../assets/models/Mutant.fbx")) {
        std::cerr << "No se pudo cargar el personaje.\n";
    }
}

// Función para dibujar un piso, techo y paredes de tablero de ajedrez
void drawCheckerboardFloor(float size, float tileSize) {
    glBegin(GL_QUADS);

    for (float x = -size; x < size; x += tileSize) {
        for (float z = -size; z < size; z += tileSize) {
            // Suelo
            if (((int)(x / tileSize) + (int)(z / tileSize)) % 2 == 0) {
                glColor3f(0.8f, 0.8f, 0.8f); // Gris claro (suelo)
            } else {
                glColor3f(0.6f, 0.6f, 0.6f); // Gris oscuro (suelo)
            }
            glVertex3f(x, 0.0f, z);
            glVertex3f(x + tileSize, 0.0f, z);
            glVertex3f(x + tileSize, 0.0f, z + tileSize);
            glVertex3f(x, 0.0f, z + tileSize);

            // Techo
            if (((int)(x / tileSize) + (int)(z / tileSize)) % 2 == 0) {
                glColor3f(0.7f, 0.7f, 0.7f); // Gris más oscuro (techo)
            } else {
                glColor3f(0.5f, 0.5f, 0.5f); // Gris más claro (techo)
            }
            glVertex3f(x, size, z);
            glVertex3f(x + tileSize, size, z);
            glVertex3f(x + tileSize, size, z + tileSize);
            glVertex3f(x, size, z + tileSize);
        }
    }

    for (float y = 0.0f; y < size; y += tileSize) {
        for (float z = -size; z < size; z += tileSize) {
            // Pared derecha
            if (((int)(y / tileSize) + (int)(z / tileSize)) % 2 == 0) {
                glColor3f(0.8f, 0.6f, 0.6f); // Color diferente
            } else {
                glColor3f(0.6f, 0.4f, 0.4f);
            }
            glVertex3f(size, y, z);
            glVertex3f(size, y + tileSize, z);
            glVertex3f(size, y + tileSize, z + tileSize);
            glVertex3f(size, y, z + tileSize);

            // Pared izquierda
            if (((int)(y / tileSize) + (int)(z / tileSize)) % 2 == 0) {
                glColor3f(0.6f, 0.6f, 0.8f);
            } else {
                glColor3f(0.4f, 0.4f, 0.6f);
            }
            glVertex3f(-size, y, z);
            glVertex3f(-size, y + tileSize, z);
            glVertex3f(-size, y + tileSize, z + tileSize);
            glVertex3f(-size, y, z + tileSize);
        }

        for (float x = -size; x < size; x += tileSize) {
            // Pared frontal
            if (((int)(y / tileSize) + (int)(x / tileSize)) % 2 == 0) {
                glColor3f(0.6f, 0.8f, 0.6f);
            } else {
                glColor3f(0.4f, 0.6f, 0.4f);
            }
            glVertex3f(x, y, size);
            glVertex3f(x + tileSize, y, size);
            glVertex3f(x + tileSize, y + tileSize, size);
            glVertex3f(x, y + tileSize, size);

            // Pared trasera
            if (((int)(y / tileSize) + (int)(x / tileSize)) % 2 == 0) {
                glColor3f(0.8f, 0.8f, 0.6f);
            } else {
                glColor3f(0.6f, 0.6f, 0.4f);
            }
            glVertex3f(x, y, -size);
            glVertex3f(x + tileSize, y, -size);
            glVertex3f(x + tileSize, y + tileSize, -size);
            glVertex3f(x, y + tileSize, -size);
        }
    }

    glEnd();
}

// Función para dibujar un cubo
void drawCube(float size) {
    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex3f(-size, -size, size); // Inferior izquierda
    glVertex3f(size, -size, size);  // Inferior derecha
    glVertex3f(size, size, size);   // Superior derecha
    glVertex3f(-size, size, size);  // Superior izquierda

    // Cara trasera
    
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, -size, -size);

    // Cara izquierda
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, size, -size);

    // Cara derecha
    
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);

    // Cara superior
    glColor3f(0.5f, 0.5f, 0.5f); // Rosa
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);

    // Cara inferior
    glColor3f(0.5f, 0.5f, 0.5f); // Azul claro
    glVertex3f(-size, -size,-size);
    glVertex3f(size,-size,-size);
    glVertex3f(size,-size,size);
    glVertex3f(-size,-size,size);

    glEnd();
}


