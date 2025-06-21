#include "../../include/core/trippy_visuals.h"
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include <random>
#include <cmath>
#include <unistd.h>

// Simple Trippy Visuals namespace
namespace TrippyVisuals {
    // Global variables for trippy effects
    GLFWwindow* window = nullptr;
    
    // Time and animation variables
    float time = 0.0f;
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Generate psychedelic color
    glm::vec3 generatePsychedelicColor(float time, float offset) {
        float r = 0.5f + 0.5f * sin(time * 1.1f + offset);
        float g = 0.5f + 0.5f * sin(time * 1.3f + offset + 2.094f);
        float b = 0.5f + 0.5f * sin(time * 1.7f + offset + 4.189f);
        
        return glm::vec3(r, g, b);
    }
    
    // Draw a simple cube
    void drawCube(float size) {
        float s = size * 0.5f;
        
        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-s, -s, s);
        glVertex3f(s, -s, s);
        glVertex3f(s, s, s);
        glVertex3f(-s, s, s);
        
        // Back face
        glVertex3f(-s, -s, -s);
        glVertex3f(-s, s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, -s, -s);
        
        // Top face
        glVertex3f(-s, s, -s);
        glVertex3f(-s, s, s);
        glVertex3f(s, s, s);
        glVertex3f(s, s, -s);
        
        // Bottom face
        glVertex3f(-s, -s, -s);
        glVertex3f(s, -s, -s);
        glVertex3f(s, -s, s);
        glVertex3f(-s, -s, s);
        
        // Right face
        glVertex3f(s, -s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, s, s);
        glVertex3f(s, -s, s);
        
        // Left face
        glVertex3f(-s, -s, -s);
        glVertex3f(-s, -s, s);
        glVertex3f(-s, s, s);
        glVertex3f(-s, s, -s);
        glEnd();
    }
    
    // Draw a simple sphere
    void drawSphere(float radius, int slices, int stacks) {
        for (int i = 0; i < stacks; i++) {
            float lat0 = M_PI * (-0.5f + (float)i / stacks);
            float z0 = sin(lat0);
            float zr0 = cos(lat0);
            
            float lat1 = M_PI * (-0.5f + (float)(i + 1) / stacks);
            float z1 = sin(lat1);
            float zr1 = cos(lat1);
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; j++) {
                float lng = 2 * M_PI * (float)j / slices;
                float x = cos(lng);
                float y = sin(lng);
                
                glNormal3f(x * zr0, y * zr0, z0);
                glVertex3f(x * zr0 * radius, y * zr0 * radius, z0 * radius);
                glNormal3f(x * zr1, y * zr1, z1);
                glVertex3f(x * zr1 * radius, y * zr1 * radius, z1 * radius);
            }
            glEnd();
        }
    }
    
    // Draw floating objects
    void drawFloatingObjects() {
        int numObjects = 6;
        for (int i = 0; i < numObjects; i++) {
            float angle = (float)i / numObjects * 2.0f * M_PI;
            float radius = 4.0f + sin(time * 0.5f + i) * 1.0f;
            float x = cos(angle + time * 0.5f) * radius;
            float z = sin(angle + time * 0.5f) * radius;
            float y = sin(time * 1.0f + i * 0.5f) * 1.5f;
            
            float scale = 0.3f + sin(time * 0.3f + i) * 0.1f;
            
            glm::vec3 color = generatePsychedelicColor(time, i * 0.5f);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            glScalef(scale, scale, scale);
            glColor3f(color.r, color.g, color.b);
            
            if (i % 2 == 0) {
                drawCube(1.0f);
            } else {
                drawSphere(0.5f, 8, 8);
            }
            
            glPopMatrix();
        }
    }
    
    // Draw psychedelic background
    void drawBackground() {
        glBegin(GL_QUADS);
        for (int x = -10; x < 10; x++) {
            for (int z = -10; z < 10; z++) {
                float x1 = x * 1.0f;
                float z1 = z * 1.0f;
                float x2 = (x + 1) * 1.0f;
                float z2 = (z + 1) * 1.0f;
                
                glm::vec3 color1 = generatePsychedelicColor(time, x * 0.1f + z * 0.1f);
                glm::vec3 color2 = generatePsychedelicColor(time, x2 * 0.1f + z * 0.1f);
                glm::vec3 color3 = generatePsychedelicColor(time, x2 * 0.1f + z2 * 0.1f);
                glm::vec3 color4 = generatePsychedelicColor(time, x1 * 0.1f + z2 * 0.1f);
                
                float y = sin(time * 0.5f + x * 0.3f) * cos(time * 0.3f + z * 0.2f) * 0.5f;
                
                glColor3f(color1.r, color1.g, color1.b);
                glVertex3f(x1, y, z1);
                
                glColor3f(color2.r, color2.g, color2.b);
                glVertex3f(x2, y, z1);
                
                glColor3f(color3.r, color3.g, color3.b);
                glVertex3f(x2, y, z2);
                
                glColor3f(color4.r, color4.g, color4.b);
                glVertex3f(x1, y, z2);
            }
        }
        glEnd();
    }
    
    // Initialize simple trippy visuals
    bool initialize() {
        // Change to project root directory
        chdir("..");
        
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        
        // Create fullscreen window
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(mode->width, mode->height, "🎭 TRIPPY VISUALS 🎭", monitor, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return false;
        }
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        return true;
    }
    
    // Main render loop
    void renderLoop() {
        while (!glfwWindowShouldClose(window)) {
            // Update time
            time += 0.016f;
            
            // Clear screen
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Set up projection
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0f, 1920.0f/1080.0f, 0.1f, 100.0f);
            
            // Set up view
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            float cameraDistance = 12.0f + sin(time * 0.2f) * 2.0f;
            gluLookAt(0.0f, 3.0f, cameraDistance,
                      0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f);
            
            // Draw psychedelic effects
            drawBackground();
            drawFloatingObjects();
            
            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
            
            // Check for exit key
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                break;
            }
        }
    }
    
    // Cleanup
    void cleanup() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }
    
    // Main function to run trippy visuals
    void runTrippyVisuals() {
        if (!initialize()) {
            std::cerr << "Failed to initialize Trippy Visuals" << std::endl;
            return;
        }
        
        std::cout << "🎭 TRIPPY VISUALS ACTIVATED 🎭" << std::endl;
        std::cout << "Press ESC to exit..." << std::endl;
        
        renderLoop();
        cleanup();
    }
} 