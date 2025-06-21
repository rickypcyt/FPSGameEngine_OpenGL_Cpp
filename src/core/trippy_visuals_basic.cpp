#include "../../include/core/trippy_visuals.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <cmath>
#include <unistd.h>

// Basic Trippy Visuals namespace
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
            
            drawCube(1.0f);
            
            glPopMatrix();
        }
    }
    
    // Draw psychedelic background
    void drawBackground() {
        glBegin(GL_QUADS);
        for (int x = -8; x < 8; x++) {
            for (int z = -8; z < 8; z++) {
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
    
    // Initialize basic trippy visuals
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
            glFrustum(-1.0, 1.0, -0.5625, 0.5625, 1.0, 100.0); // 16:9 aspect ratio
            
            // Set up view
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            float cameraDistance = 12.0f + sin(time * 0.2f) * 2.0f;
            glTranslatef(0.0f, -2.0f, -cameraDistance);
            
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