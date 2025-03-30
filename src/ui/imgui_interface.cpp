#include "../../include/ui/imgui_interface.h"
#include "../../include/third_party/imgui/imgui.h"
#include "../../include/third_party/imgui/imgui_impl_glfw.h"
#include "../../include/third_party/imgui/imgui_impl_opengl2.h"
#include <iostream>

namespace UI {
    void initializeImGui(GLFWwindow* window) {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL2_Init();

        // Setup style
        ImGui::StyleColorsDark();
    }

    void cleanupImGui() {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void beginImGuiFrame() {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void endImGuiFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    void renderEditorWindow(Editor::WorldEditor& editor) {
        ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        
        // Object type selection
        static int currentItem = 0;
        const char* items[] = { "Wall", "Rectangle", "Cube" };
        if (ImGui::Combo("Object Type", &currentItem, items, IM_ARRAYSIZE(items))) {
            editor.setCurrentObjectType(static_cast<Editor::ObjectType>(currentItem));
        }

        // Object list
        ImGui::Separator();
        ImGui::Text("Objects");
        ImGui::BeginChild("ObjectList", ImVec2(0, 200), true);
        
        const auto& objects = editor.getObjects();
        for (size_t i = 0; i < objects.size(); i++) {
            const auto& obj = objects[i];
            std::string label = "Object " + std::to_string(i);
            if (ImGui::Selectable(label.c_str(), i == editor.getSelectedObjectIndex())) {
                editor.selectObject(i);
            }
        }
        
        ImGui::EndChild();

        // Object properties
        if (editor.getSelectedObjectIndex() < objects.size()) {
            ImGui::Separator();
            ImGui::Text("Properties");
            
            const auto& obj = objects[editor.getSelectedObjectIndex()];
            glm::vec3 pos = obj->getPosition();
            glm::vec3 size = obj->getSize();
            
            if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) {
                obj->setPosition(pos);
            }
            if (ImGui::DragFloat3("Size", &size.x, 0.1f)) {
                obj->setSize(size);
            }
        }

        ImGui::End();
    }
} 