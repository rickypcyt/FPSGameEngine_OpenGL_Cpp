#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include "stb_image.h"
// Function to load a texture
GLuint loadTexture(const char* filepath);

extern GLuint textureID; // Add this line

void drawWall();
void drawCheckerboardFloor(float size, float tileSize);
// Function to draw the floor
void drawScene();
void drawCube(float size);
void initializeOpenGL();

#endif // RENDERER_H
