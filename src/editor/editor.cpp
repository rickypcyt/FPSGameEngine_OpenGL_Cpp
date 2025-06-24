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

// Cube implementation
Cube::Cube(const glm::vec3& position, const glm::vec3& size)
    : EditableObject(ObjectType::CUBE, position, size) {}

void Cube::render() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    glBegin(GL_QUADS);
    // Front face
    glColor3f(0.2f, 0.2f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    // Back face
    glColor3f(0.2f, 0.2f, 0.8f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    // Left face
    glColor3f(0.1f, 0.1f, 0.7f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    // Right face
    glColor3f(0.1f, 0.1f, 0.7f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    // Top face
    glColor3f(0.3f, 0.3f, 1.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    // Bottom face
    glColor3f(0.1f, 0.1f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

void Cube::renderPreview() const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(size.x, size.y, size.z);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    // Front face
    glColor4f(0.2f, 0.2f, 1.0f, 0.3f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    // Back face
    glColor4f(0.2f, 0.2f, 0.8f, 0.3f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    // Left face
    glColor4f(0.1f, 0.1f, 0.7f, 0.3f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    // Right face
    glColor4f(0.1f, 0.1f, 0.7f, 0.3f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    // Top face
    glColor4f(0.3f, 0.3f, 1.0f, 0.3f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    // Bottom face
    glColor4f(0.1f, 0.1f, 0.5f, 0.3f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}

void Cube::update() {
    // Cube-specific update logic if needed
}

// WorldEditor implementation
WorldEditor::WorldEditor()
    : selectedObjectIndex(0), isEditing(false), currentObjectType(ObjectType::WALL) {}

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
    switch (currentObjectType) {
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
        case ObjectType::CUBE: {
            Cube previewCube(position, size);
            previewCube.renderPreview();
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
        case ObjectType::CUBE:
            obj = std::make_unique<Cube>(position, size);
            break;
        default:
            return;
    }
    objects.push_back(std::move(obj));
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

} // namespace Editor