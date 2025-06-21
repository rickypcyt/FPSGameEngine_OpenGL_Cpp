#include "../../include/graphics/modern_renderer.h"
#include "../../include/core/globals.h"
#include "../../include/input/movement.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Global variables for demo
ModernRenderer* renderer = nullptr;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;

// Keyboard callback for movement
void processInput(GLFWwindow* window) {
    float cameraSpeed = 0.05f;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    renderer->setCameraPosition(cameraPos);
}

void runModernDemo() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Modern OpenGL Demo", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return;
    }
    
    // Create and initialize renderer
    renderer = new ModernRenderer();
    if (!renderer->initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return;
    }
    
    // Set up lighting
    renderer->setLightPosition(glm::vec3(2.0f, 2.0f, 2.0f));
    renderer->setLightColor(glm::vec3(1.0f, 1.0f, 1.0f));
    renderer->enableLighting(true);
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        // Clear screen
        renderer->clear();
        
        // Begin frame
        renderer->beginFrame();
        
        // Draw a floor plane
        renderer->drawPlane(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(10.0f, 10.0f));
        
        // Draw some cubes
        renderer->drawCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
        renderer->drawCube(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f));
        renderer->drawCube(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.5f));
        
        // Draw some spheres
        renderer->drawSphere(glm::vec3(0.0f, 1.0f, 2.0f), 0.5f);
        renderer->drawSphere(glm::vec3(2.0f, 1.0f, 2.0f), 0.3f);
        
        // End frame
        renderer->endFrame();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    delete renderer;
    glfwTerminate();
}

// Function to demonstrate the differences between old and new approaches
void demonstrateModernApproach() {
    std::cout << "\n=== MODERN OPENGL APPROACH ===" << std::endl;
    std::cout << "1. VAOs (Vertex Array Objects):" << std::endl;
    std::cout << "   - Store vertex attribute configurations" << std::endl;
    std::cout << "   - Set up once, reuse many times" << std::endl;
    std::cout << "   - Much faster than setting attributes each frame" << std::endl;
    
    std::cout << "\n2. VBOs (Vertex Buffer Objects):" << std::endl;
    std::cout << "   - Store vertex data in GPU memory" << std::endl;
    std::cout << "   - Upload once, render many times" << std::endl;
    std::cout << "   - No more glBegin/glEnd calls" << std::endl;
    
    std::cout << "\n3. Shaders (GLSL):" << std::endl;
    std::cout << "   - Vertex shader: Transform vertices" << std::endl;
    std::cout << "   - Fragment shader: Calculate pixel colors" << std::endl;
    std::cout << "   - GPU-accelerated lighting and effects" << std::endl;
    
    std::cout << "\n4. GLM Matrices:" << std::endl;
    std::cout << "   - glm::mat4 for transformations" << std::endl;
    std::cout << "   - glm::translate, glm::rotate, glm::scale" << std::endl;
    std::cout << "   - glm::perspective for projection" << std::endl;
    std::cout << "   - glm::lookAt for camera view" << std::endl;
    
    std::cout << "\n5. Performance Benefits:" << std::endl;
    std::cout << "   - 10-100x faster than immediate mode" << std::endl;
    std::cout << "   - GPU does all the heavy lifting" << std::endl;
    std::cout << "   - Modern games use this approach" << std::endl;
} 