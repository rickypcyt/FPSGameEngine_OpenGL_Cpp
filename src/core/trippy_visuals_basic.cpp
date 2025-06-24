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
    
    // Infinite progression system
    float infiniteProgress = 0.0f; // Never-ending progression
    float phaseBlend = 0.0f; // Smooth blending between phases
    
    // Performance optimization variables
    float lastIntensityUpdate = 0.0f;
    float cachedIntensity = 0.0f;
    
    // Smooth transition variables
    float transitionTime = 0.0f;
    float lastPhaseChange = 0.0f;
    
    // Randomization seeds for organic movement
    float seed1 = 0.0f, seed2 = 0.0f, seed3 = 0.0f;
    
    // Experience phases
    enum ExperiencePhase {
        PHASE_BLACK = 0,       // Start black
        PHASE_BLOCKS = 1,      // Colored blocks appear
        PHASE_ROTATION = 2,    // Blocks start rotating
        PHASE_STARS = 3,       // Blocks become stars
        PHASE_MULTIPLY = 4,    // Stars multiply and disappear
        PHASE_TUNNEL = 5,      // Colorful tunnel appears
        PHASE_CRAZY = 6,       // Everything spins crazily
        PHASE_INFINITY = 7     // Maximum chaos
    };
    
    // Star structure
    struct Star {
        float x, y, z;
        float size;
        float rotation;
        float color[3];
        float life;
        bool active;
    };
    
    // Block structure
    struct Block {
        float x, y, z;
        float size;
        float rotation[3];
        float color[3];
        float life;
        bool active;
    };
    
    // Global arrays for objects
    std::vector<Star> stars;
    std::vector<Block> blocks;
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Smooth random function for organic movement
    float smoothRandom(float t, float seed) {
        return sin(t * 0.5f + seed) * cos(t * 0.3f + seed * 2.0f) * sin(t * 0.7f + seed * 3.0f);
    }
    
    // Smooth interpolation function
    float smoothInterpolate(float a, float b, float t) {
        t = t * t * (3.0f - 2.0f * t); // Smoothstep
        return a + (b - a) * t;
    }
    
    // Get current experience phase
    ExperiencePhase getCurrentPhase() {
        float phaseIndex = fmod(infiniteProgress, 8.0f);
        return (ExperiencePhase)((int)phaseIndex);
    }
    
    // Get phase blend for smooth transitions
    float getPhaseBlend() {
        float phaseIndex = fmod(infiniteProgress, 8.0f);
        return phaseIndex - floor(phaseIndex);
    }
    
    // Calculate automatic intensity
    float calculateMovementIntensity() {
        if (time - lastIntensityUpdate < 0.1f) {
            return cachedIntensity;
        }
        
        float baseIntensity = infiniteProgress * 0.8f;
        float automaticIntensity = 0.0f;
        
        automaticIntensity += sin(time * 0.3f) * 0.3f;
        automaticIntensity += cos(time * 0.5f) * 0.4f;
        automaticIntensity += sin(time * 0.7f) * 0.2f;
        
        cachedIntensity = baseIntensity + automaticIntensity;
        lastIntensityUpdate = time;
        
        return cachedIntensity;
    }
    
    // Generate psychedelic color
    glm::vec3 generatePsychedelicColor(float time, float offset, float intensity = 1.0f) {
        float randOffset = smoothRandom(time * 0.1f, offset) * 0.2f;
        
        float r = 0.5f + 0.5f * sin(time * 1.2f + offset + randOffset);
        float g = 0.5f + 0.5f * sin(time * 1.5f + offset + 2.094f + randOffset);
        float b = 0.5f + 0.5f * sin(time * 1.8f + offset + 4.189f + randOffset);
        
        float colorShift = infiniteProgress * 0.2f;
        r = 0.5f + 0.5f * sin(r * M_PI + colorShift);
        g = 0.5f + 0.5f * sin(g * M_PI + colorShift + 2.094f);
        b = 0.5f + 0.5f * sin(b * M_PI + colorShift + 4.189f);
        
        float intensityFactor = 1.0f + intensity * 0.5f;
        r = std::min(1.0f, r * intensityFactor);
        g = std::min(1.0f, g * intensityFactor);
        b = std::min(1.0f, b * intensityFactor);
        
        return glm::vec3(r, g, b);
    }
    
    // Draw a star
    void drawStar(float size) {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 5; i++) {
            float angle1 = i * 2.0f * M_PI / 5.0f;
            float angle2 = (i + 2) * 2.0f * M_PI / 5.0f;
            
            float x1 = cos(angle1) * size;
            float y1 = sin(angle1) * size;
            float x2 = cos(angle2) * size;
            float y2 = sin(angle2) * size;
            
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(x1, y1, 0.0f);
            glVertex3f(x2, y2, 0.0f);
        }
        glEnd();
    }
    
    // Draw a cube
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
    
    // Initialize blocks
    void initializeBlocks() {
        blocks.clear();
        for (int i = 0; i < 8; i++) {
            Block block;
            block.x = (dis(gen) - 0.5f) * 10.0f;
            block.y = (dis(gen) - 0.5f) * 2.0f;
            block.z = (dis(gen) - 0.5f) * 10.0f;
            block.size = 0.5f + dis(gen) * 0.5f;
            block.rotation[0] = 0.0f;
            block.rotation[1] = 0.0f;
            block.rotation[2] = 0.0f;
            block.life = 0.0f;
            block.active = false;
            
            glm::vec3 color = generatePsychedelicColor(time, i * 0.5f, 1.0f);
            block.color[0] = color.r;
            block.color[1] = color.g;
            block.color[2] = color.b;
            
            blocks.push_back(block);
        }
    }
    
    // Initialize stars
    void initializeStars() {
        stars.clear();
        for (int i = 0; i < 20; i++) {
            Star star;
            star.x = (dis(gen) - 0.5f) * 15.0f;
            star.y = (dis(gen) - 0.5f) * 3.0f;
            star.z = (dis(gen) - 0.5f) * 15.0f;
            star.size = 0.3f + dis(gen) * 0.4f;
            star.rotation = 0.0f;
            star.life = 0.0f;
            star.active = false;
            
            glm::vec3 color = generatePsychedelicColor(time, i * 0.3f, 1.0f);
            star.color[0] = color.r;
            star.color[1] = color.g;
            star.color[2] = color.b;
            
            stars.push_back(star);
        }
    }
    
    // Update blocks
    void updateBlocks(float deltaTime) {
        ExperiencePhase phase = getCurrentPhase();
        
        for (auto& block : blocks) {
            if (phase >= PHASE_BLOCKS) {
                block.life += deltaTime * 0.5f;
                
                if (phase >= PHASE_ROTATION) {
                    block.rotation[0] += deltaTime * 90.0f;
                    block.rotation[1] += deltaTime * 120.0f;
                    block.rotation[2] += deltaTime * 150.0f;
                }
                
                if (phase >= PHASE_STARS) {
                    // Blocks start transforming into stars
                    block.size += deltaTime * 0.5f;
                }
            }
        }
    }
    
    // Update stars
    void updateStars(float deltaTime) {
        ExperiencePhase phase = getCurrentPhase();
        
        for (auto& star : stars) {
            if (phase >= PHASE_STARS) {
                star.life += deltaTime * 0.8f;
                star.rotation += deltaTime * 180.0f;
                
                if (phase >= PHASE_MULTIPLY) {
                    // Stars multiply and disappear randomly
                    if (dis(gen) < 0.01f) {
                        star.active = !star.active;
                    }
                    
                    if (star.active) {
                        star.size += deltaTime * 0.3f;
                    } else {
                        star.size -= deltaTime * 0.5f;
                    }
                }
            }
        }
    }
    
    // Draw blocks
    void drawBlocks() {
        ExperiencePhase phase = getCurrentPhase();
        
        for (const auto& block : blocks) {
            if (phase >= PHASE_BLOCKS && block.life > 0.0f) {
                float alpha = std::min(1.0f, block.life);
                
                glPushMatrix();
                glTranslatef(block.x, block.y, block.z);
                glRotatef(block.rotation[0], 1.0f, 0.0f, 0.0f);
                glRotatef(block.rotation[1], 0.0f, 1.0f, 0.0f);
                glRotatef(block.rotation[2], 0.0f, 0.0f, 1.0f);
                glScalef(block.size, block.size, block.size);
                
                glColor4f(block.color[0], block.color[1], block.color[2], alpha);
                drawCube(1.0f);
                
                glPopMatrix();
            }
        }
    }
    
    // Draw stars
    void drawStars() {
        ExperiencePhase phase = getCurrentPhase();
        
        for (const auto& star : stars) {
            if (phase >= PHASE_STARS && star.life > 0.0f) {
                float alpha = std::min(1.0f, star.life);
                if (phase >= PHASE_MULTIPLY && !star.active) {
                    alpha *= 0.3f;
                }
                
                glPushMatrix();
                glTranslatef(star.x, star.y, star.z);
                glRotatef(star.rotation, 0.0f, 0.0f, 1.0f);
                glScalef(star.size, star.size, star.size);
                
                glColor4f(star.color[0], star.color[1], star.color[2], alpha);
                drawStar(1.0f);
                
                glPopMatrix();
            }
        }
    }
    
    // Draw tunnel
    void drawTunnel() {
        ExperiencePhase phase = getCurrentPhase();
        if (phase >= PHASE_TUNNEL) {
            int tunnelRings = 20 + (int)(intensity * 15);
            float tunnelSpeed = 3.0f + infiniteProgress * 1.0f;
            
            for (int ring = 0; ring < tunnelRings; ring++) {
                float z = -ring * 1.5f - time * tunnelSpeed;
                float radius = 2.5f + smoothRandom(time * 0.3f, ring * 0.1f) * 0.8f + intensity * 0.8f;
                
                glBegin(GL_LINE_LOOP);
                int segments = 24 + (int)(intensity * 12);
                for (int i = 0; i < segments; i++) {
                    float angle = (float)i / segments * 2.0f * M_PI;
                    float x = cos(angle) * radius;
                    float y = sin(angle) * radius;
                    
                    // Impredecible movement
                    float randOffset = smoothRandom(time * 0.2f, ring * 0.3f + i * 0.1f) * 0.3f;
                    x += randOffset;
                    y += randOffset;
                    
                    glm::vec3 color = generatePsychedelicColor(time, ring * 0.02f + i * 0.01f, intensity);
                    glColor3f(color.r, color.g, color.b);
                    glVertex3f(x, y, z);
                }
                glEnd();
            }
        }
    }
    
    // Draw crazy spinning background
    void drawCrazyBackground() {
        ExperiencePhase phase = getCurrentPhase();
        if (phase >= PHASE_CRAZY) {
            int gridSize = 12 + (int)(intensity * 8);
            
            glBegin(GL_QUADS);
            for (int x = -gridSize; x < gridSize; x++) {
                for (int z = -gridSize; z < gridSize; z++) {
                    float x1 = x * 0.6f;
                    float z1 = z * 0.6f;
                    float x2 = (x + 1) * 0.6f;
                    float z2 = (z + 1) * 0.6f;
                    
                    glm::vec3 color1 = generatePsychedelicColor(time, x * 0.01f + z * 0.01f, intensity);
                    glm::vec3 color2 = generatePsychedelicColor(time, x2 * 0.01f + z * 0.01f, intensity);
                    glm::vec3 color3 = generatePsychedelicColor(time, x2 * 0.01f + z2 * 0.01f, intensity);
                    glm::vec3 color4 = generatePsychedelicColor(time, x1 * 0.01f + z2 * 0.01f, intensity);
                    
                    // Crazy wave patterns
                    float waveSpeed = 0.8f + infiniteProgress * 0.3f;
                    float waveAmplitude = 0.3f + infiniteProgress * 0.5f;
                    
                    float randOffset1 = smoothRandom(time * 0.1f, x * 0.2f) * 0.2f;
                    float randOffset2 = smoothRandom(time * 0.15f, z * 0.2f) * 0.2f;
                    
                    float y = sin(time * waveSpeed + x * 0.2f + randOffset1) * 
                             cos(time * waveSpeed * 0.9f + z * 0.2f + randOffset2) * waveAmplitude;
                    
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
    }
    
    // Initialize basic trippy visuals
    bool initialize() {
        chdir("..");
        
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(mode->width, mode->height, "🎭 SHOW PSICODÉLICO INFINITO 🎭", monitor, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Initialize random seeds
        seed1 = dis(gen);
        seed2 = dis(gen);
        seed3 = dis(gen);
        
        // Initialize objects
        initializeBlocks();
        initializeStars();
        
        return true;
    }
    
    // Main render loop
    void renderLoop() {
        float lastFrame = 0.0f;
        
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            if (deltaTime > 0.1f) deltaTime = 0.1f;
            
            // Update infinite progression
            time += deltaTime;
            infiniteProgress += 0.0004f; // Faster progression
            journeyProgress = fmod(infiniteProgress, 1.0f);
            
            intensity = calculateMovementIntensity();
            
            // Update objects
            updateBlocks(deltaTime);
            updateStars(deltaTime);
            
            ExperiencePhase phase = getCurrentPhase();
            
            // Set background color based on phase
            if (phase == PHASE_BLACK) {
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            } else {
                glm::vec3 bgColor = generatePsychedelicColor(time, 0.0f, intensity * 0.1f);
                glClearColor(bgColor.r * 0.1f, bgColor.g * 0.1f, bgColor.b * 0.1f, 1.0f);
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // 2D view setup
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glFrustum(-1.0, 1.0, -0.5625, 0.5625, 1.0, 100.0);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            // Camera setup with crazy rotation in later phases
            if (phase >= PHASE_CRAZY) {
                float crazyRotX = sin(time * 0.5f) * 30.0f;
                float crazyRotY = cos(time * 0.7f) * 45.0f;
                float crazyRotZ = sin(time * 0.3f) * 20.0f;
                
                glRotatef(crazyRotX, 1.0f, 0.0f, 0.0f);
                glRotatef(crazyRotY, 0.0f, 1.0f, 0.0f);
                glRotatef(crazyRotZ, 0.0f, 0.0f, 1.0f);
            } else {
                glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
            }
            
            glTranslatef(0.0f, -8.0f, -15.0f);
            
            // Draw effects based on phase
            drawCrazyBackground();
            drawBlocks();
            drawStars();
            drawTunnel();
            
            glfwSwapBuffers(window);
            glfwPollEvents();
            
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
        
        std::cout << "🎭 SHOW PSICODÉLICO INFINITO ACTIVADO 🎭" << std::endl;
        std::cout << "🌌 Experiencia progresiva: Negro → Bloques → Estrellas → Túnel → Locura" << std::endl;
        std::cout << "✨ Efectos automáticos con transiciones suaves" << std::endl;
        std::cout << "🎨 Randomización y multiplicación de objetos" << std::endl;
        std::cout << "ESC - Salir del show" << std::endl;
        std::cout << "Disfruta el viaje... 🚀" << std::endl;
        
        renderLoop();
        cleanup();
    }
} 