#pragma once
#include <glm/glm.hpp>

struct PhysicsProperties {
    float jumpForce = 8.0f;
    float gravity = 20.0f;
    float groundLevel = 0.0f;
    float airControl = 0.7f;
    float moveSpeed = 7.0f;
    float acceleration = 25.0f;
    float deceleration = 30.0f;
};

class PhysicsSystem {
public:
    void initialize();
    void update(float deltaTime);
    
    const glm::vec3& getPosition() const { return position; }
    bool isGrounded() const { return position.y <= properties.groundLevel; }
    void jump();
    void move(const glm::vec3& direction, float deltaTime);

private:
    glm::vec3 position{0.0f, 0.0f, 5.0f};
    glm::vec3 velocity{0.0f};
    PhysicsProperties properties;
    bool isJumping = false;
};
