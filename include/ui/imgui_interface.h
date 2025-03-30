#pragma once

#include <GLFW/glfw3.h>
#include "../editor/editor.h"

namespace UI {
    void initializeImGui(GLFWwindow* window);
    void cleanupImGui();
    void beginImGuiFrame();
    void endImGuiFrame();
    void renderEditorWindow(Editor::WorldEditor& editor);
} 