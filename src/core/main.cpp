#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>
#include <imgui.h>

#include "../../include/graphics/renderer.h"
#include "../../include/input/movement.h"
#include "../../include/core/globals.h"
#include "../../include/ui/cursor.h"
#include "../../include/ui/crosshair.h"
#include "../../include/input/editor_input.h"
#include "../../include/ui/imgui_interface.h"
#include "../../include/ui/fps_counter.h"
#include "../../include/core/main.h"
// #include "../include/input.h"
// #include "../include/godmode.h"

// Constants
const float TARGET_FPS = 120.0f;
const float FRAME_DURATION = 1000.0f / TARGET_FPS; // in milliseconds

// Globals
GLFWwindow* window = nullptr;
float lastFrameTime = 0.0f;
float lastTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;
FPSCounter fpsCounter;  // Add FPS counter instance

// Function Declarations
void displayFPS(float fps);
void setupProjection();
void renderText(const std::string& text, float x, float y);
void initializeGLFW();
void initializeGLEW();
void initializeGLUT(int& argc, char** argv);
void setupCallbacks();
void mainLoop();
void initializeInput(GLFWwindow* window);
void cleanupGame();
void drawUniversalCursor(GLFWwindow* window);

// Function Implementations
void displayFPS(float fps) {
    std::cout << "FPS: " << fps << std::endl;
}

void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, static_cast<float>(WIDTH) / HEIGHT, 0.1f, 100.0f); // Increased FOV to 90
    glMatrixMode(GL_MODELVIEW);
}

void renderText(const std::string& text, float x, float y) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void initializeGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Error initializing GLFW");
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(1920, 1080, "MyOpenGLGame", monitor, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Error creating GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);  // Add this line to disable vsync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, 1920, 1080);
}

void initializeGLEW() {
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Error initializing GLEW");
    }
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
}

void initializeGLUT(int& argc, char** argv) {
    glutInit(&argc, argv);
}

void setupCallbacks() {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void mainLoop() {
    using namespace std::chrono;
    auto lastFrameTimePoint = high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto frameStartTime = high_resolution_clock::now();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        // Update FPS counter
        fpsCounter.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera
        glm::vec3 cameraPosition(characterPosX, characterPosY + 1.5f, characterPosZ);
        glm::vec3 cameraTarget = cameraPosition + cameraFront;
        gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
                  cameraTarget.x, cameraTarget.y, cameraTarget.z,
                  0.0f, 1.0f, 0.0f);
        
        drawScene();
        
        // Render editor objects (siempre)
        EditorInput::worldEditor.render();
        // Render preview de placement aunque no esté en modo editor
        if (EditorInput::isPlacingObject) {
            EditorInput::updatePlacement(deltaTime, cameraPosition, cameraFront);
            EditorInput::worldEditor.renderPreview(EditorInput::placingPos, EditorInput::placingScale);
        }
        
        drawCrosshair(WIDTH, HEIGHT);
        
        if (!EditorInput::isEditorMode) {
            Movement::updateMovement(deltaTime);
        }
        
        EditorInput::update(deltaTime);

        // Render ImGui interface
        UI::beginImGuiFrame();
        
        // FPS Display Window
        {
            ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::SetWindowPos(ImVec2(10, 10));
            ImGui::SetWindowSize(ImVec2(200, 100));
            
            ImGui::Text("FPS: %.1f", fpsCounter.getCurrentFPS());
            ImGui::Text("Average: %.1f", fpsCounter.getAverageFPS());
            ImGui::Text("Min: %.1f", fpsCounter.getMinFPS());
            ImGui::Text("Max: %.1f", fpsCounter.getMaxFPS());
            
            ImGui::End();
        }

        if (EditorInput::isEditorMode) {
            UI::renderEditorWindow(EditorInput::worldEditor);
        }
        UI::endImGuiFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Frame capping
        auto frameEndTime = high_resolution_clock::now();
        duration<float, std::milli> frameDuration = frameEndTime - frameStartTime;

        if (frameDuration.count() < FRAME_DURATION) {
            std::this_thread::sleep_for(milliseconds(static_cast<int>(FRAME_DURATION - frameDuration.count())));
        }
    }
}

void initializeInput(GLFWwindow* window) {
    // Implementation of initializeInput function
}

void cleanupGame() {
    UI::cleanupImGui();
    glfwTerminate();
}

// Main game function that can be called from the menu
void startGame() {
    try {
        initializeGLFW();
        initializeGLEW();

        // int argc = 0;
        // char** argv = nullptr;
        // initializeGLUT(argc, argv); // Eliminado: glutInit solo se llama una vez en main()

        setupCallbacks();
        setupProjection();
        
        EditorInput::initialize(window);
        UI::initializeImGui(window);
        
        mainLoop();
        
        cleanupGame();
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        cleanupGame();
    }
}

void drawUniversalCursor(GLFWwindow* window) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    float radius = 7.0f;
    int segments = 24;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(xpos + dx, height - ypos + dy);
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
