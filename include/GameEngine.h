#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

class GameEngine {
public:
    static GameEngine& getInstance() {
        static GameEngine instance;
        return instance;
    }

    bool initialize(int width, int height, const char* title);
    void run();
    void cleanup();

    // Getters for engine components
    class InputManager& getInputManager() { return *inputManager; }
    class PhysicsSystem& getPhysicsSystem() { return *physicsSystem; }
    class RenderSystem& getRenderSystem() { return *renderSystem; }
    GLFWwindow* getWindow() const { return window; }

private:
    GameEngine() = default;
    ~GameEngine() = default;
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    void updateDeltaTime();
    
    GLFWwindow* window = nullptr;
    std::unique_ptr<class InputManager> inputManager;
    std::unique_ptr<class PhysicsSystem> physicsSystem;
    std::unique_ptr<class RenderSystem> renderSystem;
    
    double lastFrameTime = 0.0;
    float deltaTime = 0.0f;
    bool isRunning = false;
};
