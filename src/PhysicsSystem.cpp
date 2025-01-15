#include "../include/PhysicsSystem.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>

void PhysicsSystem::initialize() {
    position = {0.0f, 0.0f, 5.0f};
    velocity = {0.0f, 0.0f, 0.0f};
    isJumping = false;
}

void PhysicsSystem::update(float deltaTime) {
    // Apply gravity
    velocity.y -= properties.gravity * deltaTime;

    // Update position based on velocity
    position += velocity * deltaTime;

    // Handle ground collision
    if (position.y <= properties.groundLevel) {
        position.y = properties.groundLevel;
        velocity.y = 0.0f;
        isJumping = false;
    }

    // Apply air resistance (deceleration)
    float decelerationFactor = isGrounded() ? properties.deceleration : (properties.deceleration * properties.airControl);
    glm::vec3 horizontalVelocity = {velocity.x, 0.0f, velocity.z};
    float speed = glm::length(horizontalVelocity);
    if (speed > 0) {
        float drop = speed * decelerationFactor * deltaTime;
        velocity.x *= std::max(speed - drop, 0.0f) / speed;
        velocity.z *= std::max(speed - drop, 0.0f) / speed;
    }
}

void PhysicsSystem::jump() {
    if (isGrounded()) {
        velocity.y = properties.jumpForce;
        isJumping = true;
    }
}

void PhysicsSystem::move(const glm::vec3& direction, float deltaTime) {
    glm::vec3 normalizedDirection = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
    float accelerationFactor = isGrounded() ? 1.0f : properties.airControl;
    
    glm::vec3 targetVelocity = normalizedDirection * properties.moveSpeed;
    glm::vec3 velocityDiff = targetVelocity - glm::vec3(velocity.x, 0.0f, velocity.z);
    glm::vec3 accelerationVector = velocityDiff * (properties.acceleration * accelerationFactor * deltaTime);

    velocity.x += accelerationVector.x;
    velocity.z += accelerationVector.z;

    // Clamp horizontal velocity to moveSpeed
    glm::vec3 horizontalVelocity = {velocity.x, 0.0f, velocity.z};
    if (glm::length(horizontalVelocity) > properties.moveSpeed) {
        horizontalVelocity = glm::normalize(horizontalVelocity) * properties.moveSpeed;
        velocity.x = horizontalVelocity.x;
        velocity.z = horizontalVelocity.z;
    }
}
