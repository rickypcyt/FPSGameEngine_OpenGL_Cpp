#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../editor/editor.h"

namespace EditorInput {
    extern bool isEditorMode;
    extern bool isPlacingObject;
    extern bool isGodMode;
    extern glm::vec3 placementStart;
    extern glm::vec3 placementEnd;
    extern Editor::WorldEditor worldEditor;
    extern GLFWwindow* window;

    void initialize(GLFWwindow* window);
    void update(float deltaTime);
    void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
    void handleMouseClick(GLFWwindow* window, int button, int action, int mods);
    void handleMouseMove(GLFWwindow* window, double xpos, double ypos);
} 