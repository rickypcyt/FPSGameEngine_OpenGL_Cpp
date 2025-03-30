#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

#include "../../include/graphics/renderer.h"
#include "../../include/input/movement.h"
#include "../../include/core/globals.h"
#include "../../include/ui/cursor.h"
#include "../../include/ui/crosshair.h"
#include "../../include/input/editor_input.h"
#include "../../include/ui/imgui_interface.h"
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera
        glm::vec3 cameraPosition(characterPosX, characterPosY + 1.5f, characterPosZ);
        glm::vec3 cameraTarget = cameraPosition + cameraFront;
        gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
                  cameraTarget.x, cameraTarget.y, cameraTarget.z,
                  0.0f, 1.0f, 0.0f);
        
        drawScene();
        
        // Render editor objects and preview
        if (EditorInput::isEditorMode) {
            EditorInput::worldEditor.render();
            
            // Draw preview if placing object
            if (EditorInput::isPlacingObject) {
                glPushMatrix();
                glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
                glBegin(GL_LINES);
                glVertex3f(EditorInput::placementStart.x, EditorInput::placementStart.y, EditorInput::placementStart.z);
                glVertex3f(EditorInput::placementEnd.x, EditorInput::placementEnd.y, EditorInput::placementEnd.z);
                glEnd();
                glPopMatrix();
            }
        }
        
        drawCrosshair(WIDTH, HEIGHT);
        
        if (!EditorInput::isEditorMode) {
            Movement::updateMovement(deltaTime);
        }
        
        EditorInput::update(deltaTime);

        // FPS calculation
        frameCount++;
        if (currentFrame - lastTime >= 1.0f) {
            fps = frameCount;
            displayFPS(fps);
            frameCount = 0;
            lastTime += 1.0f;
        }

        // Render FPS and editor mode status
        std::ostringstream statusStream;
        statusStream << "FPS: " << fps;
        if (EditorInput::isEditorMode) {
            statusStream << " | Editor Mode";
        }
        renderText(statusStream.str(), -0.9f, 0.9f);

        // Render ImGui interface
        UI::beginImGuiFrame();
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

int main() {
    try {
        initializeGLFW();
        initializeGLEW();

        int argc = 0;
        char** argv = nullptr;
        initializeGLUT(argc, argv);

        setupCallbacks();
        setupProjection();
        
        EditorInput::initialize(window);
        UI::initializeImGui(window);
        
        mainLoop();
        
        UI::cleanupImGui();
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwTerminate();
    return 0;
}
