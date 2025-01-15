#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include "stb_image.h"
// Function to load a texture
GLuint loadTexture(const char* filepath);

extern GLuint textureID; // Add this line

// Function to draw the floor
void drawFloor();



#endif // RENDERER_H
