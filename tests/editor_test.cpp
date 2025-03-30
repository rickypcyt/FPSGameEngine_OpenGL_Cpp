#include <gtest/gtest.h>
#include <editor/editor.h>
#include <glm/glm.hpp>

class EditorTest : public ::testing::Test {
protected:
    Editor::WorldEditor worldEditor;
};

TEST_F(EditorTest, InitialState) {
    EXPECT_EQ(worldEditor.getObjects().size(), 0);
    EXPECT_EQ(worldEditor.getSelectedObjectIndex(), 0);
    EXPECT_EQ(worldEditor.getCurrentObjectType(), Editor::ObjectType::WALL);
}

TEST_F(EditorTest, AddWall) {
    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::vec3 size(1.0f, 2.0f, 0.1f);
    
    worldEditor.addObject(Editor::ObjectType::WALL, position, size);
    
    EXPECT_EQ(worldEditor.getObjects().size(), 1);
    const auto& objects = worldEditor.getObjects();
    EXPECT_NE(objects[0], nullptr);
    EXPECT_EQ(objects[0]->getType(), Editor::ObjectType::WALL);
    EXPECT_EQ(objects[0]->getPosition(), position);
    EXPECT_EQ(objects[0]->getSize(), size);
}

TEST_F(EditorTest, AddRectangle) {
    glm::vec3 position(2.0f, 3.0f, 4.0f);
    glm::vec3 size(2.0f, 1.0f, 1.0f);
    
    worldEditor.addObject(Editor::ObjectType::RECTANGLE, position, size);
    
    EXPECT_EQ(worldEditor.getObjects().size(), 1);
    const auto& objects = worldEditor.getObjects();
    EXPECT_NE(objects[0], nullptr);
    EXPECT_EQ(objects[0]->getType(), Editor::ObjectType::RECTANGLE);
    EXPECT_EQ(objects[0]->getPosition(), position);
    EXPECT_EQ(objects[0]->getSize(), size);
}

TEST_F(EditorTest, SelectObject) {
    // Add multiple objects
    worldEditor.addObject(Editor::ObjectType::WALL, glm::vec3(0.0f), glm::vec3(1.0f));
    worldEditor.addObject(Editor::ObjectType::RECTANGLE, glm::vec3(1.0f), glm::vec3(1.0f));
    
    worldEditor.selectObject(1);
    EXPECT_EQ(worldEditor.getSelectedObjectIndex(), 1);
    
    const auto& objects = worldEditor.getObjects();
    EXPECT_EQ(objects[1]->getType(), Editor::ObjectType::RECTANGLE);
}

TEST_F(EditorTest, MoveObject) {
    glm::vec3 initialPos(0.0f);
    worldEditor.addObject(Editor::ObjectType::WALL, initialPos, glm::vec3(1.0f));
    
    glm::vec3 offset(1.0f, 2.0f, 3.0f);
    worldEditor.moveSelectedObject(offset);
    
    const auto& objects = worldEditor.getObjects();
    EXPECT_EQ(objects[0]->getPosition(), offset);
}

TEST_F(EditorTest, ResizeObject) {
    glm::vec3 initialSize(1.0f);
    worldEditor.addObject(Editor::ObjectType::WALL, glm::vec3(0.0f), initialSize);
    
    glm::vec3 newSize(2.0f, 3.0f, 0.1f);
    worldEditor.resizeSelectedObject(newSize);
    
    const auto& objects = worldEditor.getObjects();
    EXPECT_EQ(objects[0]->getSize(), newSize);
}

TEST_F(EditorTest, DeleteObject) {
    worldEditor.addObject(Editor::ObjectType::WALL, glm::vec3(0.0f), glm::vec3(1.0f));
    EXPECT_EQ(worldEditor.getObjects().size(), 1);
    
    worldEditor.removeObject(0);
    EXPECT_EQ(worldEditor.getObjects().size(), 0);
}

TEST_F(EditorTest, MultipleObjects) {
    // Add multiple objects
    worldEditor.addObject(Editor::ObjectType::WALL, glm::vec3(0.0f), glm::vec3(1.0f));
    worldEditor.addObject(Editor::ObjectType::RECTANGLE, glm::vec3(1.0f), glm::vec3(1.0f));
    worldEditor.addObject(Editor::ObjectType::WALL, glm::vec3(2.0f), glm::vec3(1.0f));
    
    const auto& objects = worldEditor.getObjects();
    EXPECT_EQ(objects.size(), 3);
    
    // Test object types
    EXPECT_EQ(objects[0]->getType(), Editor::ObjectType::WALL);
    EXPECT_EQ(objects[1]->getType(), Editor::ObjectType::RECTANGLE);
    EXPECT_EQ(objects[2]->getType(), Editor::ObjectType::WALL);
} 