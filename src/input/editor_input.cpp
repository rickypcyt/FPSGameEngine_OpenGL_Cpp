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
    Editor::ObjectType placingType = Editor::ObjectType::WALL;
    glm::vec3 placingPos(0.0f);
    glm::vec3 placingRot(0.0f);
    glm::vec3 placingScale(1.0f);
    
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
                if (isEditorMode) currentObjectType = Editor::ObjectType::WALL;
                break;
                
            case GLFW_KEY_2:
                if (isEditorMode) currentObjectType = Editor::ObjectType::RECTANGLE;
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
            } else if (action == GLFW_RELEASE && isPlacingObject) {
                isPlacingObject = false;
                
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                
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
                
                placementEnd = glm::vec3(worldX, worldY, worldZ);
                
                // Calculate size and position for the new object
                glm::vec3 size = glm::abs(placementEnd - placementStart);
                glm::vec3 position = (placementStart + placementEnd) * 0.5f;
                
                // Add the new object
                worldEditor.addObject(currentObjectType, position, size);
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
        } else {
            // Show preview at current mouse position
            glm::vec3 previewSize(1.0f, 2.0f, 0.1f); // Default size for preview
            glm::vec3 previewPos(worldX, worldY, worldZ);
            worldEditor.renderPreview(previewPos, previewSize);
        }
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

    void updatePlacement(float deltaTime, const glm::vec3& cameraPos, const glm::vec3& cameraFront) {
        if (!isPlacingObject) return;
        // Oculta el cursor si está en placement
        if (window) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        static float placingDistance = 3.0f;
        static float moveSpeed = 5.0f;
        static float verticalSpeed = 3.0f;
        
        // Ajustar distancia con la rueda del mouse
        double scrollY = 0.0;
        if (window) {
            // Captura el scroll del mouse (debería implementarse un callback para esto, pero aquí lo simulamos)
            // Si tienes un callback real, actualiza placingDistance ahí
        }
        // Limita la distancia
        if (placingDistance < 1.0f) placingDistance = 1.0f;
        if (placingDistance > 20.0f) placingDistance = 20.0f;

        // Calcula la posición 3D justo donde apunta el crosshair (centro de pantalla)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double x = width / 2.0;
        double y = height / 2.0;
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLdouble modelview[16];
        GLdouble projection[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        GLdouble worldX, worldY, worldZ;
        gluUnProject(x, viewport[3] - y, placingDistance / 20.0, // Normaliza la profundidad
                     modelview, projection, viewport,
                     &worldX, &worldY, &worldZ);
        placingPos = glm::vec3(worldX, worldY, worldZ);

        // Movimiento con WASD
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) placingPos += glm::vec3(cameraFront.x, 0, cameraFront.z) * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) placingPos -= glm::vec3(cameraFront.x, 0, cameraFront.z) * moveSpeed * deltaTime;
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0,1,0)));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) placingPos += right * moveSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) placingPos -= right * moveSpeed * deltaTime;
        // Altura con Q/E
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) placingPos.y += verticalSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) placingPos.y -= verticalSpeed * deltaTime;
        // Rotación con R
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) placingRot.y += 90.0f * deltaTime;
        // Confirmar con click izquierdo o ENTER
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            worldEditor.addObject(placingType, placingPos, placingScale);
            isPlacingObject = false;
        }
        // Cancelar con ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            isPlacingObject = false;
        }
    }
} 