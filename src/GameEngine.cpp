#include "../include/GameEngine.h"
#include "../include/InputManager.h"
#include "../include/PhysicsSystem.h"
#include "../include/RenderSystem.h"
#include <iostream>
#include <chrono>
#include <thread>

bool GameEngine::initialize(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // Enable MSAA

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return false;
    }

    // Initialize systems
    inputManager = std::make_unique<InputManager>();
    physicsSystem = std::make_unique<PhysicsSystem>();
    renderSystem = std::make_unique<RenderSystem>();

    inputManager->initialize(window);
    physicsSystem->initialize();
    renderSystem->initialize();

    // Enable vertical sync
    glfwSwapInterval(1);
    
    // Enable depth testing and multisampling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    isRunning = true;
    return true;
}

void GameEngine::run() {
    const float targetFPS = 144.0f;
    const float targetFrameTime = 1.0f / targetFPS;

    while (isRunning && !glfwWindowShouldClose(window)) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        updateDeltaTime();
        
        // Update systems
        inputManager->update();
        
        // Handle movement input
        glm::vec3 moveDir(0.0f);
        if (inputManager->isKeyPressed(GLFW_KEY_W)) moveDir += inputManager->getCameraFront();
        if (inputManager->isKeyPressed(GLFW_KEY_S)) moveDir -= inputManager->getCameraFront();
        if (inputManager->isKeyPressed(GLFW_KEY_D)) {
            glm::vec3 right = glm::normalize(glm::cross(inputManager->getCameraFront(), inputManager->getCameraUp()));
            moveDir += right;
        }
        if (inputManager->isKeyPressed(GLFW_KEY_A)) {
            glm::vec3 right = glm::normalize(glm::cross(inputManager->getCameraFront(), inputManager->getCameraUp()));
            moveDir -= right;
        }
        if (inputManager->isKeyPressed(GLFW_KEY_SPACE) && physicsSystem->isGrounded()) {
            physicsSystem->jump();
        }

        if (glm::length(moveDir) > 0) {
            moveDir = glm::normalize(moveDir);
            moveDir.y = 0; // Keep movement horizontal
            physicsSystem->move(moveDir, deltaTime);
        }

        physicsSystem->update(deltaTime);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderSystem->render(
            physicsSystem->getPosition(),
            inputManager->getCameraFront(),
            inputManager->getCameraUp()
        );

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Frame timing
        auto frameEnd = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();
        
        if (frameTime < targetFrameTime) {
            std::this_thread::sleep_for(std::chrono::duration<float>(targetFrameTime - frameTime));
        }
    }
}

void GameEngine::updateDeltaTime() {
    double currentFrame = glfwGetTime();
    deltaTime = static_cast<float>(currentFrame - lastFrameTime);
    lastFrameTime = currentFrame;
}

void GameEngine::cleanup() {
    renderSystem->cleanup();
    glfwTerminate();
}
