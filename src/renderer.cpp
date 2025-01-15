#include <GL/gl.h>
#include "../include/renderer.h"

void drawFloor() {
    float size = 20.0f;    // Size for the floor
    float tileSize = 2.0f; // Size of each tile for the checkerboard pattern

    // Draw the checkerboard floor
    for (float x = -size; x < size; x += tileSize) {
        for (float z = -size; z < size; z += tileSize) {
            // Alternate colors based on position
            if (((int)(x / tileSize) + (int)(z / tileSize)) % 2 == 0) {
                glColor3f(0.8f, 0.8f, 0.8f); // Light gray
            } else {
                glColor3f(0.6f, 0.6f, 0.6f); // Dark gray
            }

            glBegin(GL_QUADS);
            glVertex3f(x, 0.0f, z);
            glVertex3f(x + tileSize, 0.0f, z);
            glVertex3f(x + tileSize, 0.0f, z + tileSize);
            glVertex3f(x, 0.0f, z + tileSize);
            glEnd();
        }
    }

    // Draw a wall in green
    glColor3f(0.0f, 0.7f, 0.0f); // Green color for the wall
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 0.0f, -5.0f); // Bottom left
    glVertex3f(1.0f, 0.0f, -5.0f);  // Bottom right
    glVertex3f(1.0f, 3.0f, -5.0f);  // Top right
    glVertex3f(-1.0f, 3.0f, -5.0f); // Top left
    glEnd();
}