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
    CUBE,
    HOUSE,      // New predefined objects
    TOWER,
    BRIDGE
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
    glm::vec3 getColor() const { return color; }

    void setPosition(const glm::vec3& pos) { position = pos; }
    void setSize(const glm::vec3& s) { size = s; }
    void setColor(const glm::vec3& c) { color = c; }

protected:
    ObjectType type;
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 color;
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

// New PredefinedObject class for complex objects
class PredefinedObject : public EditableObject {
public:
    PredefinedObject(ObjectType type, const glm::vec3& position, const glm::vec3& size);
    void render() const override;
    void renderPreview() const override;
    void update() override;

    // Additional properties for predefined objects
    void setWallThickness(float thickness) { wallThickness = thickness; }
    void setRoofHeight(float height) { roofHeight = height; }
    void setWindowCount(int count) { windowCount = count; }
    void setDoorWidth(float width) { doorWidth = width; }

    float getWallThickness() const { return wallThickness; }
    float getRoofHeight() const { return roofHeight; }
    int getWindowCount() const { return windowCount; }
    float getDoorWidth() const { return doorWidth; }

private:
    float wallThickness;
    float roofHeight;
    int windowCount;
    float doorWidth;
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

    // Inventory system
    void selectInventoryItem(size_t index);
    void placeSelectedItem(const glm::vec3& position, const glm::vec3& size);
    void cancelPlacement();
    bool isPlacingObject() const { return isPlacing; }
    ObjectType getSelectedInventoryItem() const { return selectedInventoryItem; }
    const std::vector<ObjectType>& getInventoryItems() const { return inventoryItems; }
    size_t getSelectedInventoryIndex() const { return selectedInventoryIndex; }

    // New methods for predefined objects
    void setSelectedObjectColor(const glm::vec3& color);
    void setSelectedObjectWallThickness(float thickness);
    void setSelectedObjectRoofHeight(float height);
    void setSelectedObjectWindowCount(int count);
    void setSelectedObjectDoorWidth(float width);

    // Getters
    const std::vector<std::unique_ptr<EditableObject>>& getObjects() const { return objects; }
    size_t getSelectedObjectIndex() const { return selectedObjectIndex; }
    ObjectType getCurrentObjectType() const { return currentObjectType; }

    // Move constructor and move assignment operator
    WorldEditor(WorldEditor&& other) noexcept
        : objects(std::move(other.objects))
        , selectedObjectIndex(other.selectedObjectIndex)
        , currentObjectType(other.currentObjectType)
        , inventoryItems(std::move(other.inventoryItems))
        , selectedInventoryIndex(other.selectedInventoryIndex)
        , selectedInventoryItem(other.selectedInventoryItem)
        , isPlacing(other.isPlacing)
        , previewPosition(other.previewPosition)
        , previewSize(other.previewSize) {}

    WorldEditor& operator=(WorldEditor&& other) noexcept {
        if (this != &other) {
            objects = std::move(other.objects);
            selectedObjectIndex = other.selectedObjectIndex;
            currentObjectType = other.currentObjectType;
            inventoryItems = std::move(other.inventoryItems);
            selectedInventoryIndex = other.selectedInventoryIndex;
            selectedInventoryItem = other.selectedInventoryItem;
            isPlacing = other.isPlacing;
            previewPosition = other.previewPosition;
            previewSize = other.previewSize;
        }
        return *this;
    }

private:
    std::vector<std::unique_ptr<EditableObject>> objects;
    size_t selectedObjectIndex;
    bool isEditing;
    ObjectType currentObjectType;

    // Inventory system members
    std::vector<ObjectType> inventoryItems;
    size_t selectedInventoryIndex;
    ObjectType selectedInventoryItem;
    bool isPlacing;
    glm::vec3 previewPosition;
    glm::vec3 previewSize;
};

} // namespace Editor 