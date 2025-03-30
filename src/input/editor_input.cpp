#include "../../include/input/editor_input.h"
#include "../../include/core/globals.h"
#include "../../include/ui/cursor.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glu.h>
#include <iostream>

namespace EditorInput {
    bool isEditorMode = false;
    bool isPlacingObject = false;
    bool isGodMode = false;
    glm::vec3 placementStart;
    glm::vec3 placementEnd;
    Editor::WorldEditor worldEditor;
    GLFWwindow* window = nullptr;
    
    // Editor state
    Editor::ObjectType currentObjectType = Editor::ObjectType::WALL;
    
    void initialize(GLFWwindow* win) {
        window = win;
        worldEditor = Editor::WorldEditor();
    }
    
    void handleKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action != GLFW_PRESS) return;
        
        switch (key) {
            case GLFW_KEY_TAB:
                isEditorMode = !isEditorMode;
                if (isEditorMode) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                } else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                break;
                
            case GLFW_KEY_1:
                if (isEditorMode) worldEditor.selectInventoryItem(0);
                break;
                
            case GLFW_KEY_2:
                if (isEditorMode) worldEditor.selectInventoryItem(1);
                break;
                
            case GLFW_KEY_3:
                if (isEditorMode) worldEditor.selectInventoryItem(2);
                break;
                
            case GLFW_KEY_4:
                if (isEditorMode) worldEditor.selectInventoryItem(3);
                break;
                
            case GLFW_KEY_5:
                if (isEditorMode) worldEditor.selectInventoryItem(4);
                break;
                
            case GLFW_KEY_P:
                if (isEditorMode && worldEditor.isPlacingObject()) {
                    worldEditor.placeSelectedItem(placementEnd, glm::vec3(1.0f, 2.0f, 0.1f));
                    isPlacingObject = false;
                }
                break;
                
            case GLFW_KEY_X:
                if (isEditorMode && worldEditor.isPlacingObject()) {
                    worldEditor.cancelPlacement();
                    isPlacingObject = false;
                }
                break;
                
            case GLFW_KEY_DELETE:
                if (isEditorMode) {
                    size_t selectedIndex = worldEditor.getSelectedObjectIndex();
                    worldEditor.removeObject(selectedIndex);
                }
                break;
        }
    }
    
    void handleMouseClick(GLFWwindow* window, int button, int action, int mods) {
        if (!isEditorMode) return;
        
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                isPlacingObject = true;
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                
                // Convert screen coordinates to world coordinates
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                GLdouble modelview[16];
                GLdouble projection[16];
                glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
                glGetDoublev(GL_PROJECTION_MATRIX, projection);
                
                GLdouble worldX, worldY, worldZ;
                gluUnProject(xpos, viewport[3] - ypos, 0.0,
                           modelview, projection, viewport,
                           &worldX, &worldY, &worldZ);
                
                placementStart = glm::vec3(worldX, worldY, worldZ);
            }
        }
    }
    
    void handleMouseMove(GLFWwindow* window, double xpos, double ypos) {
        if (!isEditorMode) return;
        
        // Update preview of object being placed
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLdouble modelview[16];
        GLdouble projection[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        
        GLdouble worldX, worldY, worldZ;
        gluUnProject(xpos, viewport[3] - ypos, 0.0,
                    modelview, projection, viewport,
                    &worldX, &worldY, &worldZ);
        
        if (isPlacingObject) {
            placementEnd = glm::vec3(worldX, worldY, worldZ);
        }
        
        // Show preview at current mouse position
        glm::vec3 previewSize(1.0f, 2.0f, 0.1f); // Default size for preview
        glm::vec3 previewPos(worldX, worldY, worldZ);
        worldEditor.renderPreview(previewPos, previewSize);
    }
    
    void update(float deltaTime) {
        if (!window) return;

        // Toggle editor mode with M key
        static bool wasMPressed = false;
        bool isMPressed = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
        if (isMPressed && !wasMPressed) {
            isEditorMode = !isEditorMode;
            if (isEditorMode) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        wasMPressed = isMPressed;

        // Toggle god mode with G key
        static bool wasGPressed = false;
        bool isGPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
        if (isGPressed && !wasGPressed) {
            isGodMode = !isGodMode;
        }
        wasGPressed = isGPressed;

        if (!isEditorMode) return;
        
        // Update editor state
        worldEditor.update();
    }
} 