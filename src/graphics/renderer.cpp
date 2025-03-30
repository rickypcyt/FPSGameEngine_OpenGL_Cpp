#include <GL/glew.h>  // Must be included first
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>   // Add for std::cerr and std::endl
#include "../../include/graphics/renderer.h"
#include "../../include/graphics/lights.h"
#include "../../include/third_party/stb_image.h"

// Function to load a texture
GLuint loadTexture(const char* filepath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Load image data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) {
            format = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
        stbi_image_free(data);
        return 0;
    }
    
    return textureID;
}

// Global texture ID for the floor
GLuint floorTextureID = 0;

// Modified drawCheckerboardFloor function to use textures
void drawCheckerboardFloor(float size, float tileSize) {
    if (floorTextureID == 0) {
        // Load the texture if not already loaded
        floorTextureID = loadTexture("assets/textures/floor.jpg");
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTextureID);
    
    glBegin(GL_QUADS);
    
    // Draw floor with texture coordinates
    for (float x = -size; x < size; x += tileSize) {
        for (float z = -size; z < size; z += tileSize) {
            float texX = (x + size) / (2 * size);
            float texZ = (z + size) / (2 * size);
            
            glTexCoord2f(texX, texZ);
            glVertex3f(x, 0.0f, z);
            glTexCoord2f(texX + tileSize/(2*size), texZ);
            glVertex3f(x + tileSize, 0.0f, z);
            glTexCoord2f(texX + tileSize/(2*size), texZ + tileSize/(2*size));
            glVertex3f(x + tileSize, 0.0f, z + tileSize);
            glTexCoord2f(texX, texZ + tileSize/(2*size));
            glVertex3f(x, 0.0f, z + tileSize);
        }
    }
    
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
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
    glColor3f(1.0f, 0.5f, 0.5f); // Rosa
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);

    // Cara inferior
    glColor3f(0.5f, 0.5f, 1.0f); // Azul claro
    glVertex3f(-size, -size,-size);
    glVertex3f(size,-size,-size);
    glVertex3f(size,-size,size);
    glVertex3f(-size,-size,size);

    glEnd();
}

// Función principal de dibujo
void drawScene() {
    drawCheckerboardFloor(20.0f, 1.0f); // Dibujar el piso de ajedrez
    drawCube(2.5);                     // Dibujar el cubo que rodea al jugador
}

// Configuración de iluminación (si se desea)


// Llamar a esta función en tu código de inicialización para configurar el estado de OpenGL
void initializeOpenGL() {
    glEnable(GL_DEPTH_TEST); // Habilitar pruebas de profundidad para un renderizado adecuado
    setupLighting();         // Configurar iluminación si se desea
}
