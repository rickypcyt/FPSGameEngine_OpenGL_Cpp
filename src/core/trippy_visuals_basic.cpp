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
    float journeyProgress = 0.0f; // 0.0 to 1.0 - represents journey progress
    float intensity = 0.0f; // Current intensity level
    
    // Journey phases
    enum JourneyPhase {
        PHASE_LAUNCH = 0,      // 0.0 - 0.2: Gentle start
        PHASE_ORBIT = 1,       // 0.2 - 0.4: Orbital effects
        PHASE_NEBULA = 2,      // 0.4 - 0.6: Color explosions
        PHASE_WORMHOLE = 3,    // 0.6 - 0.8: Tunnel effects
        PHASE_GALACTIC = 4     // 0.8 - 1.0: Maximum intensity
    };
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Generate psychedelic color with intensity
    glm::vec3 generatePsychedelicColor(float time, float offset, float intensity = 1.0f) {
        float r = 0.5f + 0.5f * sin(time * (1.1f + intensity * 0.5f) + offset);
        float g = 0.5f + 0.5f * sin(time * (1.3f + intensity * 0.7f) + offset + 2.094f);
        float b = 0.5f + 0.5f * sin(time * (1.7f + intensity * 0.9f) + offset + 4.189f);
        
        // Add intensity-based color saturation
        r = std::min(1.0f, r * (1.0f + intensity * 0.5f));
        g = std::min(1.0f, g * (1.0f + intensity * 0.5f));
        b = std::min(1.0f, b * (1.0f + intensity * 0.5f));
        
        return glm::vec3(r, g, b);
    }
    
    // Get current journey phase
    JourneyPhase getCurrentPhase() {
        if (journeyProgress < 0.2f) return PHASE_LAUNCH;
        if (journeyProgress < 0.4f) return PHASE_ORBIT;
        if (journeyProgress < 0.6f) return PHASE_NEBULA;
        if (journeyProgress < 0.8f) return PHASE_WORMHOLE;
        return PHASE_GALACTIC;
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
    
    // Draw floating objects with progression
    void drawFloatingObjects() {
        JourneyPhase phase = getCurrentPhase();
        int numObjects = 6 + (int)(intensity * 10); // More objects as intensity increases
        
        for (int i = 0; i < numObjects; i++) {
            float angle = (float)i / numObjects * 2.0f * M_PI;
            float radius = 4.0f + sin(time * 0.5f + i) * 1.0f;
            
            // Phase-specific movement patterns
            float x, y, z;
            switch (phase) {
                case PHASE_LAUNCH:
                    x = cos(angle + time * 0.3f) * radius;
                    z = sin(angle + time * 0.3f) * radius;
                    y = sin(time * 0.8f + i * 0.5f) * 1.0f;
                    break;
                case PHASE_ORBIT:
                    x = cos(angle + time * 0.8f) * radius;
                    z = sin(angle + time * 0.8f) * radius;
                    y = sin(time * 1.2f + i * 0.3f) * 2.0f;
                    break;
                case PHASE_NEBULA:
                    x = cos(angle + time * 1.2f) * (radius + sin(time * 0.7f) * 2.0f);
                    z = sin(angle + time * 1.2f) * (radius + cos(time * 0.7f) * 2.0f);
                    y = sin(time * 1.5f + i * 0.4f) * 3.0f;
                    break;
                case PHASE_WORMHOLE:
                    x = cos(angle + time * 2.0f) * (radius + sin(time * 1.0f) * 3.0f);
                    z = sin(angle + time * 2.0f) * (radius + cos(time * 1.0f) * 3.0f);
                    y = sin(time * 2.5f + i * 0.6f) * 4.0f;
                    break;
                case PHASE_GALACTIC:
                    x = cos(angle + time * 3.0f) * (radius + sin(time * 1.5f) * 4.0f);
                    z = sin(angle + time * 3.0f) * (radius + cos(time * 1.5f) * 4.0f);
                    y = sin(time * 3.5f + i * 0.8f) * 5.0f;
                    break;
            }
            
            float scale = 0.3f + sin(time * 0.3f + i) * 0.1f + intensity * 0.2f;
            
            glm::vec3 color = generatePsychedelicColor(time, i * 0.5f, intensity);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            glScalef(scale, scale, scale);
            
            // Add rotation based on phase
            float rotSpeed = 0.5f + intensity * 2.0f;
            glRotatef(time * rotSpeed * 57.3f, 1.0f, 0.5f, 0.3f);
            glRotatef(time * rotSpeed * 0.7f * 57.3f, 0.3f, 1.0f, 0.5f);
            
            glColor3f(color.r, color.g, color.b);
            drawCube(1.0f);
            
            glPopMatrix();
        }
    }
    
    // Draw psychedelic background with progression
    void drawBackground() {
        JourneyPhase phase = getCurrentPhase();
        int gridSize = 8 + (int)(intensity * 4); // Larger grid as intensity increases
        
        glBegin(GL_QUADS);
        for (int x = -gridSize; x < gridSize; x++) {
            for (int z = -gridSize; z < gridSize; z++) {
                float x1 = x * 1.0f;
                float z1 = z * 1.0f;
                float x2 = (x + 1) * 1.0f;
                float z2 = (z + 1) * 1.0f;
                
                glm::vec3 color1 = generatePsychedelicColor(time, x * 0.1f + z * 0.1f, intensity);
                glm::vec3 color2 = generatePsychedelicColor(time, x2 * 0.1f + z * 0.1f, intensity);
                glm::vec3 color3 = generatePsychedelicColor(time, x2 * 0.1f + z2 * 0.1f, intensity);
                glm::vec3 color4 = generatePsychedelicColor(time, x1 * 0.1f + z2 * 0.1f, intensity);
                
                // Phase-specific wave patterns
                float y;
                switch (phase) {
                    case PHASE_LAUNCH:
                        y = sin(time * 0.5f + x * 0.3f) * cos(time * 0.3f + z * 0.2f) * 0.3f;
                        break;
                    case PHASE_ORBIT:
                        y = sin(time * 0.8f + x * 0.4f) * cos(time * 0.5f + z * 0.3f) * 0.6f;
                        break;
                    case PHASE_NEBULA:
                        y = sin(time * 1.2f + x * 0.6f) * cos(time * 0.8f + z * 0.5f) * 1.0f;
                        break;
                    case PHASE_WORMHOLE:
                        y = sin(time * 1.8f + x * 0.8f) * cos(time * 1.2f + z * 0.7f) * 1.5f;
                        break;
                    case PHASE_GALACTIC:
                        y = sin(time * 2.5f + x * 1.0f) * cos(time * 1.8f + z * 0.9f) * 2.0f;
                        break;
                }
                
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
    
    // Draw wormhole tunnel effect
    void drawWormholeTunnel() {
        if (getCurrentPhase() >= PHASE_WORMHOLE) {
            int tunnelRings = 15 + (int)(intensity * 10);
            for (int ring = 0; ring < tunnelRings; ring++) {
                float z = -ring * 2.0f - time * (3.0f + intensity * 2.0f);
                float radius = 1.5f + sin(time * 0.5f + ring * 0.3f) * 0.3f + intensity * 0.5f;
                
                glBegin(GL_LINE_LOOP);
                int segments = 16 + (int)(intensity * 8);
                for (int i = 0; i < segments; i++) {
                    float angle = (float)i / segments * 2.0f * M_PI;
                    float x = cos(angle) * radius;
                    float y = sin(angle) * radius;
                    
                    glm::vec3 color = generatePsychedelicColor(time, ring * 0.2f + i * 0.1f, intensity);
                    glColor3f(color.r, color.g, color.b);
                    glVertex3f(x, y, z);
                }
                glEnd();
            }
        }
    }
    
    // Draw nebula particles
    void drawNebulaParticles() {
        if (getCurrentPhase() >= PHASE_NEBULA) {
            int numParticles = 50 + (int)(intensity * 100);
            glPointSize(2.0f + intensity * 3.0f);
            glBegin(GL_POINTS);
            
            for (int i = 0; i < numParticles; i++) {
                float x = (dis(gen) - 0.5f) * 20.0f;
                float y = (dis(gen) - 0.5f) * 20.0f;
                float z = (dis(gen) - 0.5f) * 20.0f;
                
                // Animate particles
                x += sin(time * 0.5f + i * 0.1f) * 2.0f;
                y += cos(time * 0.7f + i * 0.15f) * 2.0f;
                z += sin(time * 0.3f + i * 0.2f) * 2.0f;
                
                glm::vec3 color = generatePsychedelicColor(time, i * 0.1f, intensity);
                glColor3f(color.r, color.g, color.b);
                glVertex3f(x, y, z);
            }
            glEnd();
        }
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
        window = glfwCreateWindow(mode->width, mode->height, "🎭 GALACTIC TRIPPY VISUALS 🎭", monitor, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // Enable depth testing and blending
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        return true;
    }
    
    // Main render loop
    void renderLoop() {
        while (!glfwWindowShouldClose(window)) {
            // Update time and progress
            time += 0.016f;
            journeyProgress += 0.001f; // Slowly advance through the journey
            if (journeyProgress > 1.0f) journeyProgress = 1.0f;
            
            // Calculate intensity based on progress
            intensity = journeyProgress * 2.0f; // 0.0 to 2.0
            
            // Clear screen
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Set up projection
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-1.0, 1.0, -0.5625, 0.5625, 1.0, 100.0);
            
            // Set up view with dynamic camera
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            JourneyPhase phase = getCurrentPhase();
            float cameraDistance = 12.0f + sin(time * 0.2f) * 2.0f;
            float cameraY = -2.0f + sin(time * 0.3f) * intensity;
            float cameraX = sin(time * 0.1f) * intensity * 0.5f;
            
            glTranslatef(cameraX, cameraY, -cameraDistance);
            
            // Add camera rotation based on phase
            if (phase >= PHASE_WORMHOLE) {
                glRotatef(sin(time * 0.5f) * intensity * 5.0f, 0.0f, 0.0f, 1.0f);
            }
            
            // Draw psychedelic effects
            drawBackground();
            drawFloatingObjects();
            drawNebulaParticles();
            drawWormholeTunnel();
            
            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
            
            // Check for exit key
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                break;
            }
            
            // Speed up progression with spacebar
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                journeyProgress += 0.005f;
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
        
        std::cout << "🎭 GALACTIC TRIPPY VISUALS ACTIVATED 🎭" << std::endl;
        std::cout << "🚀 Journey through the galaxy begins..." << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  SPACE - Speed up journey progression" << std::endl;
        std::cout << "  ESC - Exit the experience" << std::endl;
        std::cout << "Press ESC to exit..." << std::endl;
        
        renderLoop();
        cleanup();
    }
} 