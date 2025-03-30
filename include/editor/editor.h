#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "../graphics/renderer.h"

namespace Editor {

// Enum for different types of objects that can be placed
enum class ObjectType {
    WALL,
    RECTANGLE,
    CUBE
};

// Base class for all editable objects
class EditableObject {
public:
    EditableObject(ObjectType type, const glm::vec3& position, const glm::vec3& size);
    virtual ~EditableObject() = default;

    virtual void render() const = 0;
    virtual void renderPreview() const = 0;  // New method for preview rendering
    virtual void update() = 0;

    // Getters and setters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getSize() const { return size; }
    ObjectType getType() const { return type; }

    void setPosition(const glm::vec3& pos) { position = pos; }
    void setSize(const glm::vec3& s) { size = s; }

protected:
    ObjectType type;
    glm::vec3 position;
    glm::vec3 size;
};

// Specific object types
class Wall : public EditableObject {
public:
    Wall(const glm::vec3& position, const glm::vec3& size);
    void render() const override;
    void renderPreview() const override;
    void update() override;
};

class Rectangle : public EditableObject {
public:
    Rectangle(const glm::vec3& position, const glm::vec3& size);
    void render() const override;
    void renderPreview() const override;
    void update() override;
};

// Main editor class
class WorldEditor {
public:
    WorldEditor();
    ~WorldEditor();

    void update();
    void render() const;
    void renderPreview(const glm::vec3& position, const glm::vec3& size) const;
    
    // Editor operations
    void addObject(ObjectType type, const glm::vec3& position, const glm::vec3& size);
    void removeObject(size_t index);
    void selectObject(size_t index);
    void moveSelectedObject(const glm::vec3& offset);
    void resizeSelectedObject(const glm::vec3& newSize);
    void setCurrentObjectType(ObjectType type) { currentObjectType = type; }

    // Getters
    const std::vector<std::unique_ptr<EditableObject>>& getObjects() const { return objects; }
    size_t getSelectedObjectIndex() const { return selectedObjectIndex; }
    ObjectType getCurrentObjectType() const { return currentObjectType; }

    // Move constructor and move assignment operator
    WorldEditor(WorldEditor&& other) noexcept
        : objects(std::move(other.objects))
        , selectedObjectIndex(other.selectedObjectIndex)
        , currentObjectType(other.currentObjectType) {}

    WorldEditor& operator=(WorldEditor&& other) noexcept {
        if (this != &other) {
            objects = std::move(other.objects);
            selectedObjectIndex = other.selectedObjectIndex;
            currentObjectType = other.currentObjectType;
        }
        return *this;
    }

private:
    std::vector<std::unique_ptr<EditableObject>> objects;
    size_t selectedObjectIndex;
    bool isEditing;
    ObjectType currentObjectType;
};

} // namespace Editor 