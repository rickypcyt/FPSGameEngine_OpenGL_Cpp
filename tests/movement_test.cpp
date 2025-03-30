#include <gtest/gtest.h>
#include <input/movement.h>
#include <core/globals.h>
#include <glm/glm.hpp>

// Declare the global movement state
extern MovementState moveState;

class MovementTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset global state before each test
        characterPosX = 0.0f;
        characterPosY = 0.0f;
        characterPosZ = 0.0f;
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        moveSpeed = 5.0f;
        deltaTime = 0.016f; // Simulate 60 FPS
        
        // Reset movement state
        moveState = MovementState();
    }
};

TEST_F(MovementTest, ForwardMovement) {
    moveState.moveForward = true;
    Movement::updateMovement(deltaTime);
    
    EXPECT_NEAR(characterPosZ, -0.08f, 0.001f); // moveSpeed * deltaTime
    EXPECT_EQ(characterPosX, 0.0f);
    EXPECT_EQ(characterPosY, 0.0f);
}

TEST_F(MovementTest, BackwardMovement) {
    moveState.moveBackward = true;
    Movement::updateMovement(deltaTime);
    
    EXPECT_NEAR(characterPosZ, 0.08f, 0.001f);
    EXPECT_EQ(characterPosX, 0.0f);
    EXPECT_EQ(characterPosY, 0.0f);
}

TEST_F(MovementTest, RightMovement) {
    moveState.moveRight = true;
    Movement::updateMovement(deltaTime);
    
    EXPECT_NEAR(characterPosX, 0.08f, 0.001f);
    EXPECT_EQ(characterPosZ, 0.0f);
    EXPECT_EQ(characterPosY, 0.0f);
}

TEST_F(MovementTest, LeftMovement) {
    moveState.moveLeft = true;
    Movement::updateMovement(deltaTime);
    
    EXPECT_NEAR(characterPosX, -0.08f, 0.001f);
    EXPECT_EQ(characterPosZ, 0.0f);
    EXPECT_EQ(characterPosY, 0.0f);
}

TEST_F(MovementTest, DiagonalMovement) {
    moveState.moveForward = true;
    moveState.moveRight = true;
    Movement::updateMovement(deltaTime);
    
    EXPECT_NEAR(characterPosX, 0.056f, 0.001f); // moveSpeed * deltaTime * cos(45°)
    EXPECT_NEAR(characterPosZ, -0.056f, 0.001f); // moveSpeed * deltaTime * sin(45°)
    EXPECT_EQ(characterPosY, 0.0f);
}

TEST_F(MovementTest, JumpMovement) {
    moveState.isJumping = true;
    moveState.verticalVelocity = 5.0f;
    
    Movement::updateJump(deltaTime);
    
    EXPECT_NEAR(characterPosY, 0.08f, 0.001f); // verticalVelocity * deltaTime
    EXPECT_NEAR(moveState.verticalVelocity, 4.84f, 0.001f); // 5.0f - gravity * deltaTime
}

TEST_F(MovementTest, RunningSpeed) {
    moveState.moveForward = true;
    moveState.isRunning = true;
    Movement::updateMovement(deltaTime);
    
    EXPECT_NEAR(characterPosZ, -0.16f, 0.001f); // moveSpeed * 2 * deltaTime
} 