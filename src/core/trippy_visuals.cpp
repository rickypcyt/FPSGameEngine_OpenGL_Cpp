#include "../../include/core/trippy_visuals.h"
#include "../../include/graphics/modern_renderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Trippy Visuals namespace
namespace TrippyVisuals {
    // Global variables for trippy effects
    ModernRenderer* renderer = nullptr;
    GLFWwindow* window = nullptr;
    
    // Time and animation variables
    float time = 0.0f;
    float colorTime = 0.0f;
    float morphTime = 0.0f;
    float meltTime = 0.0f;
    
    // Color cycling variables
    glm::vec3 currentColor(1.0f, 0.0f, 0.0f);
    glm::vec3 targetColor(0.0f, 1.0f, 0.0f);
    float colorTransition = 0.0f;
    
    // Morphing and melting variables
    std::vector<glm::vec3> morphingVertices;
    std::vector<float> meltFactors;
    std::vector<glm::vec3> originalPositions;
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    // Psychedelic effect parameters
    struct PsychedelicParams {
        float waveIntensity = 2.0f;
        float colorSpeed = 1.5f;
        float morphSpeed = 0.8f;
        float meltSpeed = 1.2f;
        float rotationSpeed = 0.5f;
        float scalePulse = 0.3f;
        float distortionAmount = 0.1f;
    } params;
    
    // Parameters for smooth randomization
    struct RandomizedParams {
        float fractalScale = 4.0f;
        float fractalRotation = 0.0f;
        float cameraTheta = 0.0f, cameraPhi = 0.0f, cameraRadius = 18.0f;
        float colorSeed = 0.0f;
        float starTwinkle = 0.0f;
        float fractalDepth = 3.0f;
        float fractalJitter = 0.0f;
    } randParams;
    
    // Initialize trippy visuals
    bool initialize() {
        // Change to project root directory for shader loading
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
        
        // Create and initialize renderer
        renderer = new ModernRenderer();
        if (!renderer->initialize()) {
            std::cerr << "Failed to initialize renderer" << std::endl;
            return false;
        }
        
        // Set up psychedelic lighting
        renderer->setLightPosition(glm::vec3(0.0f, 5.0f, 0.0f));
        renderer->setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
        renderer->enableLighting(true);
        
        // Initialize morphing vertices for melting effect
        initializeMorphingVertices();
        
        // Initialize starfield
        initStars();
        
        return true;
    }
    
    // Initialize vertices for morphing effects
    void initializeMorphingVertices() {
        // Create a smaller grid of vertices for morphing to avoid memory issues
        int gridSize = 10; // Reduced from 20
        morphingVertices.clear();
        originalPositions.clear();
        meltFactors.clear();
        
        for (int x = -gridSize; x <= gridSize; x++) {
            for (int z = -gridSize; z <= gridSize; z++) {
                float xPos = x * 0.8f; // Increased spacing
                float zPos = z * 0.8f;
                float yPos = 0.0f;
                
                originalPositions.push_back(glm::vec3(xPos, yPos, zPos));
                morphingVertices.push_back(glm::vec3(xPos, yPos, zPos));
                meltFactors.push_back(dis(gen));
            }
        }
    }
    
    // Generate psychedelic color
    glm::vec3 generatePsychedelicColor(float time, float offset) {
        float r = 0.5f + 0.5f * sin(time * 1.1f + offset);
        float g = 0.5f + 0.5f * sin(time * 1.3f + offset + 2.094f); // 2π/3
        float b = 0.5f + 0.5f * sin(time * 1.7f + offset + 4.189f); // 4π/3
        
        return glm::vec3(r, g, b);
    }
    
    // Apply wave distortion to vertices
    void applyWaveDistortion() {
        for (size_t i = 0; i < morphingVertices.size(); i++) {
            glm::vec3& vertex = morphingVertices[i];
            glm::vec3 original = originalPositions[i];
            
            // Create wave effect
            float wave1 = sin(time * params.waveIntensity + original.x * 0.5f) * 
                         cos(time * params.waveIntensity * 0.7f + original.z * 0.3f);
            float wave2 = cos(time * params.waveIntensity * 0.5f + original.x * 0.2f) * 
                         sin(time * params.waveIntensity * 1.3f + original.z * 0.8f);
            
            vertex.y = original.y + (wave1 + wave2) * 0.5f * params.distortionAmount;
            
            // Add melting effect
            float meltFactor = meltFactors[i];
            vertex.y -= sin(time * params.meltSpeed + meltFactor * 10.0f) * 0.3f * params.meltSpeed;
        }
    }
    
    // Draw melting grid
    void drawMeltingGrid() {
        glBegin(GL_TRIANGLES);
        
        int gridSize = 10; // Match the reduced grid size
        for (int x = -gridSize; x < gridSize; x++) {
            for (int z = -gridSize; z < gridSize; z++) {
                int index1 = (x + gridSize) * (2 * gridSize + 1) + (z + gridSize);
                int index2 = (x + 1 + gridSize) * (2 * gridSize + 1) + (z + gridSize);
                int index3 = (x + gridSize) * (2 * gridSize + 1) + (z + 1 + gridSize);
                int index4 = (x + 1 + gridSize) * (2 * gridSize + 1) + (z + 1 + gridSize);
                
                if (index1 < morphingVertices.size() && index2 < morphingVertices.size() && 
                    index3 < morphingVertices.size() && index4 < morphingVertices.size()) {
                    
                    glm::vec3 color1 = generatePsychedelicColor(time, index1 * 0.1f);
                    glm::vec3 color2 = generatePsychedelicColor(time, index2 * 0.1f);
                    glm::vec3 color3 = generatePsychedelicColor(time, index3 * 0.1f);
                    glm::vec3 color4 = generatePsychedelicColor(time, index4 * 0.1f);
                    
                    // First triangle
                    glColor3f(color1.r, color1.g, color1.b);
                    glVertex3f(morphingVertices[index1].x, morphingVertices[index1].y, morphingVertices[index1].z);
                    
                    glColor3f(color2.r, color2.g, color2.b);
                    glVertex3f(morphingVertices[index2].x, morphingVertices[index2].y, morphingVertices[index2].z);
                    
                    glColor3f(color3.r, color3.g, color3.b);
                    glVertex3f(morphingVertices[index3].x, morphingVertices[index3].y, morphingVertices[index3].z);
                    
                    // Second triangle
                    glColor3f(color2.r, color2.g, color2.b);
                    glVertex3f(morphingVertices[index2].x, morphingVertices[index2].y, morphingVertices[index2].z);
                    
                    glColor3f(color4.r, color4.g, color4.b);
                    glVertex3f(morphingVertices[index4].x, morphingVertices[index4].y, morphingVertices[index4].z);
                    
                    glColor3f(color3.r, color3.g, color3.b);
                    glVertex3f(morphingVertices[index3].x, morphingVertices[index3].y, morphingVertices[index3].z);
                }
            }
        }
        glEnd();
    }
    
    // Draw morphing spheres
    void drawMorphingSpheres() {
        int numSpheres = 8; // Reduced from 15
        for (int i = 0; i < numSpheres; i++) {
            float angle = (float)i / numSpheres * 2.0f * M_PI;
            float radius = 6.0f + sin(time * 0.5f + i) * 1.5f; // Reduced radius
            float x = cos(angle + time * params.rotationSpeed) * radius;
            float z = sin(angle + time * params.rotationSpeed) * radius;
            float y = sin(time * 1.2f + i * 0.5f) * 2.0f;
            
            // Morphing scale
            float scale = 0.4f + sin(time * params.scalePulse + i) * 0.2f; // Reduced scale
            
            // Psychedelic color
            glm::vec3 color = generatePsychedelicColor(time, i * 0.5f);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            glScalef(scale, scale, scale);
            
            // Draw sphere with color
            glColor3f(color.r, color.g, color.b);
            renderer->drawSphere(glm::vec3(0.0f), 1.0f);
            
            glPopMatrix();
        }
    }
    
    // Draw floating cubes with morphing
    void drawMorphingCubes() {
        int numCubes = 6; // Reduced from 12
        for (int i = 0; i < numCubes; i++) {
            float angle = (float)i / numCubes * 2.0f * M_PI;
            float radius = 4.0f + cos(time * 0.3f + i) * 1.0f; // Reduced radius
            float x = cos(angle + time * params.rotationSpeed * 0.7f) * radius;
            float z = sin(angle + time * params.rotationSpeed * 0.7f) * radius;
            float y = cos(time * 0.8f + i * 0.7f) * 1.5f; // Reduced height
            
            // Morphing rotation
            float rotX = time * 0.5f + i * 0.3f;
            float rotY = time * 0.7f + i * 0.5f;
            float rotZ = time * 0.3f + i * 0.2f;
            
            // Morphing scale
            float scale = 0.3f + sin(time * params.scalePulse * 1.5f + i) * 0.15f; // Reduced scale
            
            // Psychedelic color
            glm::vec3 color = generatePsychedelicColor(time, i * 0.8f + 2.0f);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            glRotatef(rotX * 57.3f, 1.0f, 0.0f, 0.0f);
            glRotatef(rotY * 57.3f, 0.0f, 1.0f, 0.0f);
            glRotatef(rotZ * 57.3f, 0.0f, 0.0f, 1.0f);
            glScalef(scale, scale, scale);
            
            // Draw cube with color
            glColor3f(color.r, color.g, color.b);
            renderer->drawCube(glm::vec3(0.0f), glm::vec3(1.0f));
            
            glPopMatrix();
        }
    }
    
    // Draw psychedelic tunnel effect
    void drawPsychedelicTunnel() {
        int tunnelRings = 10; // Reduced from 20
        for (int ring = 0; ring < tunnelRings; ring++) {
            float z = -ring * 3.0f - time * 2.0f; // Slower movement
            float radius = 1.5f + sin(time * 0.5f + ring * 0.3f) * 0.3f; // Smaller radius
            
            glBegin(GL_LINE_LOOP);
            int segments = 16; // Reduced from 32
            for (int i = 0; i < segments; i++) {
                float angle = (float)i / segments * 2.0f * M_PI;
                float x = cos(angle) * radius;
                float y = sin(angle) * radius;
                
                glm::vec3 color = generatePsychedelicColor(time, ring * 0.2f + i * 0.1f);
                glColor3f(color.r, color.g, color.b);
                glVertex3f(x, y, z);
            }
            glEnd();
        }
    }
    
    // Smooth random walk helper
    template<typename T>
    T smoothRandomWalk(T value, float speed, float minVal, float maxVal) {
        float delta = (dis(gen) - 0.5f) * speed;
        value += delta;
        if (value < minVal) value = minVal;
        if (value > maxVal) value = maxVal;
        return value;
    }
    
    // Update randomized parameters for smooth, trippy changes
    void updateRandomizedParams() {
        randParams.fractalScale = smoothRandomWalk(randParams.fractalScale, 0.01f, 2.5f, 6.0f);
        randParams.fractalRotation = smoothRandomWalk(randParams.fractalRotation, 0.02f, 0.0f, 2.0f * M_PI);
        randParams.cameraTheta = smoothRandomWalk(randParams.cameraTheta, 0.01f, 0.0f, M_PI);
        randParams.cameraPhi = smoothRandomWalk(randParams.cameraPhi, 0.01f, 0.0f, 2.0f * M_PI);
        randParams.cameraRadius = smoothRandomWalk(randParams.cameraRadius, 0.05f, 13.0f, 22.0f);
        randParams.colorSeed = smoothRandomWalk(randParams.colorSeed, 0.01f, 0.0f, 100.0f);
        randParams.starTwinkle = smoothRandomWalk(randParams.starTwinkle, 0.02f, 0.0f, 10.0f);
        randParams.fractalDepth = smoothRandomWalk(randParams.fractalDepth, 0.05f, 2.0f, 4.0f);
        randParams.fractalJitter = smoothRandomWalk(randParams.fractalJitter, 0.01f, 0.0f, 0.25f);
    }
    
    // Draw a Sierpinski Tetrahedron fractal (recursive)
    void drawTetrahedron(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, int depth) {
        if (depth == 0) {
            glBegin(GL_TRIANGLES);
            glm::vec3 color = generatePsychedelicColor(time, (p1.x + p2.y + p3.z + p4.x) * 0.1f + randParams.colorSeed);
            glColor3f(color.r, color.g, color.b);
            // Face 1
            glVertex3fv(&p1.x); glVertex3fv(&p2.x); glVertex3fv(&p3.x);
            // Face 2
            glVertex3fv(&p1.x); glVertex3fv(&p2.x); glVertex3fv(&p4.x);
            // Face 3
            glVertex3fv(&p1.x); glVertex3fv(&p3.x); glVertex3fv(&p4.x);
            // Face 4
            glVertex3fv(&p2.x); glVertex3fv(&p3.x); glVertex3fv(&p4.x);
            glEnd();
            return;
        }
        glm::vec3 mid12 = (p1 + p2) * 0.5f;
        glm::vec3 mid13 = (p1 + p3) * 0.5f;
        glm::vec3 mid14 = (p1 + p4) * 0.5f;
        glm::vec3 mid23 = (p2 + p3) * 0.5f;
        glm::vec3 mid24 = (p2 + p4) * 0.5f;
        glm::vec3 mid34 = (p3 + p4) * 0.5f;
        // Add jitter for trippy effect
        float jitter = randParams.fractalJitter;
        mid12 += glm::vec3(dis(gen)-0.5f, dis(gen)-0.5f, dis(gen)-0.5f) * jitter;
        mid13 += glm::vec3(dis(gen)-0.5f, dis(gen)-0.5f, dis(gen)-0.5f) * jitter;
        mid14 += glm::vec3(dis(gen)-0.5f, dis(gen)-0.5f, dis(gen)-0.5f) * jitter;
        mid23 += glm::vec3(dis(gen)-0.5f, dis(gen)-0.5f, dis(gen)-0.5f) * jitter;
        mid24 += glm::vec3(dis(gen)-0.5f, dis(gen)-0.5f, dis(gen)-0.5f) * jitter;
        mid34 += glm::vec3(dis(gen)-0.5f, dis(gen)-0.5f, dis(gen)-0.5f) * jitter;
        drawTetrahedron(p1, mid12, mid13, mid14, depth-1);
        drawTetrahedron(mid12, p2, mid23, mid24, depth-1);
        drawTetrahedron(mid13, mid23, p3, mid34, depth-1);
        drawTetrahedron(mid14, mid24, mid34, p4, depth-1);
    }

    void drawFractalTrip() {
        glPushMatrix();
        glRotatef(randParams.fractalRotation * 57.3f, 0.3f, 1.0f, 0.7f);
        float s = randParams.fractalScale;
        int depth = (int)randParams.fractalDepth;
        glm::vec3 p1(0, s, 0);
        glm::vec3 p2(-s, -s, s);
        glm::vec3 p3(s, -s, s);
        glm::vec3 p4(0, -s, -s);
        drawTetrahedron(p1, p2, p3, p4, depth);
        glPopMatrix();
    }

    // Draw a simple 3D starfield
    const int STAR_COUNT = 120;
    std::vector<glm::vec3> stars;
    void initStars() {
        stars.clear();
        for (int i = 0; i < STAR_COUNT; ++i) {
            float r = 40.0f + dis(gen) * 60.0f;
            float theta = dis(gen) * M_PI;
            float phi = dis(gen) * 2.0f * M_PI;
            float x = r * sin(theta) * cos(phi);
            float y = r * sin(theta) * sin(phi);
            float z = r * cos(theta);
            stars.push_back(glm::vec3(x, y, z));
        }
    }
    void drawStarfield() {
        glPointSize(2.0f + 2.0f * sin(time + randParams.starTwinkle));
        glBegin(GL_POINTS);
        for (auto& s : stars) {
            float twinkle = 0.7f + 0.3f * sin(time * 2.0f + s.x + s.y + s.z);
            glColor3f(twinkle, twinkle, twinkle + 0.2f * sin(time + s.x));
            glVertex3f(s.x, s.y, s.z);
        }
        glEnd();
    }
    
    // Main render loop
    void renderLoop() {
        while (!glfwWindowShouldClose(window)) {
            // Clear screen
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set camera to a fixed position
            renderer->setCameraPosition(glm::vec3(0.0f, 0.0f, 10.0f));

            // Begin frame
            renderer->beginFrame();
            renderer->enableLighting(false); // No lighting, pure color
            renderer->drawCube(glm::vec3(0.0f), glm::vec3(2.0f)); // Red cube (if shader allows)
            renderer->endFrame();

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
        if (renderer) {
            delete renderer;
            renderer = nullptr;
        }
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