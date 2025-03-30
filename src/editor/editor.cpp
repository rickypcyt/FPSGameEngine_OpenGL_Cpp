#include "../../include/editor/editor.h"
#include <GL/gl.h>
#include <memory>

namespace Editor {

// EditableObject implementation
EditableObject::EditableObject(ObjectType type, const glm::vec3& position, const glm::vec3& size)
    : type(type), position(position), size(size) {}

void EditableObject::update() {
    // Base implementation - can be overridden by derived classes
}

// Wall implementation
Wall::Wall(const glm::vec3& position, const glm::vec3& size)
    : EditableObject(ObjectType::WALL, position, size) {}

void Wall::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    glBegin(GL_QUADS);
    // Front face
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    // Back face
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    
    glPopMatrix();
}

void Wall::renderPreview() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_QUADS);
    // Front face
    glColor4f(0.0f, 1.0f, 0.0f, 0.3f); // Semi-transparent green
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    // Back face
    glColor4f(0.0f, 0.8f, 0.0f, 0.3f); // Slightly darker green
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    
    // Draw outline
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // Solid green outline
    glBegin(GL_LINE_LOOP);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    
    glDisable(GL_BLEND);
    glPopMatrix();
}

void Wall::update() {
    // Wall-specific update logic if needed
}

// Rectangle implementation
Rectangle::Rectangle(const glm::vec3& position, const glm::vec3& size)
    : EditableObject(ObjectType::RECTANGLE, position, size) {}

void Rectangle::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    glBegin(GL_QUADS);
    // Top face
    glColor3f(0.6f, 0.8f, 0.6f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    
    // Bottom face
    glColor3f(0.4f, 0.6f, 0.4f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    
    glPopMatrix();
}

void Rectangle::renderPreview() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_QUADS);
    // Top face
    glColor4f(0.0f, 1.0f, 0.0f, 0.3f); // Semi-transparent green
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    
    // Bottom face
    glColor4f(0.0f, 0.8f, 0.0f, 0.3f); // Slightly darker green
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    
    // Draw outline
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // Solid green outline
    glBegin(GL_LINE_LOOP);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();
    
    glDisable(GL_BLEND);
    glPopMatrix();
}

void Rectangle::update() {
    // Rectangle-specific update logic if needed
}

// PredefinedObject implementation
PredefinedObject::PredefinedObject(ObjectType type, const glm::vec3& position, const glm::vec3& size)
    : EditableObject(type, position, size) {
    // Initialize default values
    wallThickness = 0.2f;
    roofHeight = 1.0f;
    windowCount = 2;
    doorWidth = 1.0f;
    color = glm::vec3(0.8f, 0.8f, 0.8f); // Default gray color
}

void PredefinedObject::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    switch (type) {
        case ObjectType::HOUSE: {
            // Draw house body
            glColor3f(color.x, color.y, color.z);
            glBegin(GL_QUADS);
            // Front face
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            
            // Back face
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glEnd();
            
            // Draw roof
            glColor3f(0.6f, 0.3f, 0.0f); // Brown color for roof
            glBegin(GL_TRIANGLES);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(0.0f, 0.5f + roofHeight, 0.0f);
            
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.0f, 0.5f + roofHeight, 0.0f);
            glEnd();
            
            // Draw windows
            glColor3f(0.7f, 0.9f, 1.0f); // Light blue for windows
            float windowSpacing = 1.0f / (windowCount + 1);
            float windowSize = 0.2f;
            
            for (int i = 0; i < windowCount; ++i) {
                float x = -0.5f + (i + 1) * windowSpacing;
                glBegin(GL_QUADS);
                glVertex3f(x - windowSize/2, 0.0f, 0.51f);
                glVertex3f(x + windowSize/2, 0.0f, 0.51f);
                glVertex3f(x + windowSize/2, windowSize, 0.51f);
                glVertex3f(x - windowSize/2, windowSize, 0.51f);
                glEnd();
            }
            
            // Draw door
            glColor3f(0.4f, 0.2f, 0.0f); // Brown color for door
            glBegin(GL_QUADS);
            glVertex3f(-doorWidth/2, -0.5f, 0.51f);
            glVertex3f(doorWidth/2, -0.5f, 0.51f);
            glVertex3f(doorWidth/2, 0.0f, 0.51f);
            glVertex3f(-doorWidth/2, 0.0f, 0.51f);
            glEnd();
            break;
        }
        
        case ObjectType::TOWER: {
            // Draw tower body
            glColor3f(color.x, color.y, color.z);
            glBegin(GL_QUADS);
            // Front face
            glVertex3f(-0.3f, -0.5f, 0.3f);
            glVertex3f(0.3f, -0.5f, 0.3f);
            glVertex3f(0.3f, 0.5f, 0.3f);
            glVertex3f(-0.3f, 0.5f, 0.3f);
            
            // Back face
            glVertex3f(-0.3f, -0.5f, -0.3f);
            glVertex3f(-0.3f, 0.5f, -0.3f);
            glVertex3f(0.3f, 0.5f, -0.3f);
            glVertex3f(0.3f, -0.5f, -0.3f);
            glEnd();
            
            // Draw windows on each side
            glColor3f(0.7f, 0.9f, 1.0f);
            float windowSize = 0.15f;
            
            // Front windows
            for (int i = 0; i < windowCount; ++i) {
                float y = -0.4f + (i * 0.8f / (windowCount - 1));
                glBegin(GL_QUADS);
                glVertex3f(-windowSize/2, y - windowSize/2, 0.31f);
                glVertex3f(windowSize/2, y - windowSize/2, 0.31f);
                glVertex3f(windowSize/2, y + windowSize/2, 0.31f);
                glVertex3f(-windowSize/2, y + windowSize/2, 0.31f);
                glEnd();
            }
            break;
        }
        
        case ObjectType::BRIDGE: {
            // Draw bridge body
            glColor3f(color.x, color.y, color.z);
            glBegin(GL_QUADS);
            // Top face
            glVertex3f(-0.5f, 0.0f, -0.2f);
            glVertex3f(-0.5f, 0.0f, 0.2f);
            glVertex3f(0.5f, 0.0f, 0.2f);
            glVertex3f(0.5f, 0.0f, -0.2f);
            
            // Bottom face
            glVertex3f(-0.5f, -0.1f, -0.2f);
            glVertex3f(0.5f, -0.1f, -0.2f);
            glVertex3f(0.5f, -0.1f, 0.2f);
            glVertex3f(-0.5f, -0.1f, 0.2f);
            glEnd();
            
            // Draw supports
            glColor3f(0.4f, 0.4f, 0.4f);
            float supportWidth = 0.1f;
            float supportSpacing = 1.0f / (windowCount + 1);
            
            for (int i = 0; i < windowCount; ++i) {
                float x = -0.5f + (i + 1) * supportSpacing;
                glBegin(GL_QUADS);
                glVertex3f(x - supportWidth/2, -0.1f, -0.2f);
                glVertex3f(x - supportWidth/2, -0.1f, 0.2f);
                glVertex3f(x + supportWidth/2, -0.1f, 0.2f);
                glVertex3f(x + supportWidth/2, -0.1f, -0.2f);
                glEnd();
            }
            break;
        }
    }
    
    glPopMatrix();
}

void PredefinedObject::renderPreview() const {
    // Similar to render() but with transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    
    // Draw semi-transparent preview
    glColor4f(0.0f, 1.0f, 0.0f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    
    glDisable(GL_BLEND);
    glPopMatrix();
}

void PredefinedObject::update() {
    // Update logic for predefined objects if needed
}

// WorldEditor implementation
WorldEditor::WorldEditor()
    : selectedObjectIndex(0)
    , isEditing(false)
    , currentObjectType(ObjectType::WALL)
    , selectedInventoryIndex(0)
    , selectedInventoryItem(ObjectType::WALL)
    , isPlacing(false)
    , previewPosition(0.0f)
    , previewSize(1.0f) {
    // Initialize inventory with available objects
    inventoryItems = {
        ObjectType::WALL,
        ObjectType::RECTANGLE,
        ObjectType::HOUSE,
        ObjectType::TOWER,
        ObjectType::BRIDGE
    };
}

WorldEditor::~WorldEditor() = default;

void WorldEditor::update() {
    for (auto& obj : objects) {
        obj->update();
    }
}

void WorldEditor::render() const {
    for (const auto& obj : objects) {
        obj->render();
    }
}

void WorldEditor::renderPreview(const glm::vec3& position, const glm::vec3& size) const {
    if (!isPlacing) return;

    switch (selectedInventoryItem) {
        case ObjectType::WALL: {
            Wall previewWall(position, size);
            previewWall.renderPreview();
            break;
        }
        case ObjectType::RECTANGLE: {
            Rectangle previewRect(position, size);
            previewRect.renderPreview();
            break;
        }
        case ObjectType::HOUSE:
        case ObjectType::TOWER:
        case ObjectType::BRIDGE: {
            PredefinedObject previewObj(selectedInventoryItem, position, size);
            previewObj.renderPreview();
            break;
        }
        default:
            break;
    }
}

void WorldEditor::addObject(ObjectType type, const glm::vec3& position, const glm::vec3& size) {
    std::unique_ptr<EditableObject> obj;
    
    switch (type) {
        case ObjectType::WALL:
            obj = std::make_unique<Wall>(position, size);
            break;
        case ObjectType::RECTANGLE:
            obj = std::make_unique<Rectangle>(position, size);
            break;
        case ObjectType::HOUSE:
        case ObjectType::TOWER:
        case ObjectType::BRIDGE:
            obj = std::make_unique<PredefinedObject>(type, position, size);
            break;
        default:
            return;
    }
    
    objects.push_back(std::move(obj));
    selectedObjectIndex = objects.size() - 1;
}

void WorldEditor::removeObject(size_t index) {
    if (index < objects.size()) {
        objects.erase(objects.begin() + index);
        if (selectedObjectIndex >= objects.size()) {
            selectedObjectIndex = objects.size() > 0 ? objects.size() - 1 : 0;
        }
    }
}

void WorldEditor::selectObject(size_t index) {
    if (index < objects.size()) {
        selectedObjectIndex = index;
    }
}

void WorldEditor::moveSelectedObject(const glm::vec3& offset) {
    if (selectedObjectIndex < objects.size()) {
        auto& obj = objects[selectedObjectIndex];
        obj->setPosition(obj->getPosition() + offset);
    }
}

void WorldEditor::resizeSelectedObject(const glm::vec3& newSize) {
    if (selectedObjectIndex < objects.size()) {
        objects[selectedObjectIndex]->setSize(newSize);
    }
}

void WorldEditor::setSelectedObjectColor(const glm::vec3& color) {
    if (selectedObjectIndex < objects.size()) {
        objects[selectedObjectIndex]->setColor(color);
    }
}

void WorldEditor::setSelectedObjectWallThickness(float thickness) {
    if (selectedObjectIndex < objects.size()) {
        if (auto* predefined = dynamic_cast<PredefinedObject*>(objects[selectedObjectIndex].get())) {
            predefined->setWallThickness(thickness);
        }
    }
}

void WorldEditor::setSelectedObjectRoofHeight(float height) {
    if (selectedObjectIndex < objects.size()) {
        if (auto* predefined = dynamic_cast<PredefinedObject*>(objects[selectedObjectIndex].get())) {
            predefined->setRoofHeight(height);
        }
    }
}

void WorldEditor::setSelectedObjectWindowCount(int count) {
    if (selectedObjectIndex < objects.size()) {
        if (auto* predefined = dynamic_cast<PredefinedObject*>(objects[selectedObjectIndex].get())) {
            predefined->setWindowCount(count);
        }
    }
}

void WorldEditor::setSelectedObjectDoorWidth(float width) {
    if (selectedObjectIndex < objects.size()) {
        if (auto* predefined = dynamic_cast<PredefinedObject*>(objects[selectedObjectIndex].get())) {
            predefined->setDoorWidth(width);
        }
    }
}

void WorldEditor::selectInventoryItem(size_t index) {
    if (index < inventoryItems.size()) {
        selectedInventoryIndex = index;
        selectedInventoryItem = inventoryItems[index];
        isPlacing = true;
        previewPosition = glm::vec3(0.0f);
        previewSize = glm::vec3(1.0f);
    }
}

void WorldEditor::placeSelectedItem(const glm::vec3& position, const glm::vec3& size) {
    if (isPlacing) {
        addObject(selectedInventoryItem, position, size);
        isPlacing = false;
    }
}

void WorldEditor::cancelPlacement() {
    isPlacing = false;
}

} // namespace Editor