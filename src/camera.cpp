#include "../include/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

const float PI = 3.14159265358979323846f;

FPSCamera::FPSCamera(const glm::vec3& pos, float pitchDeg, float yawDeg)
    : position(pos),
      pitch(glm::radians(pitchDeg)),
      yaw(glm::radians(yawDeg)),
      MIN_PITCH(-89.0f * PI / 180.0f),
      MAX_PITCH(89.0f * PI / 180.0f),
      worldUp(0.0f, 1.0f, 0.0f),
      speed(5.0f), // Increased movement speed for a Quake-like feel
      jumpHeight(5.0f), // Jump height
      gravity(-9.81f), // Gravity
      isJumping(false),
      velocity(0.0f) // Initialize vertical velocity
{
    updateVectors();
}

void FPSCamera::updateVectors() {
    pitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);
    
    // Calculate forward vector
    forwardVec.x = cos(yaw) * cos(pitch);
    forwardVec.y = sin(pitch);
    forwardVec.z = sin(yaw) * cos(pitch);
    forwardVec = glm::normalize(forwardVec);
    
    // Calculate right and up vectors
    rightVec = glm::normalize(glm::cross(forwardVec, worldUp));
    upVec = glm::normalize(glm::cross(rightVec, forwardVec));
}

void FPSCamera::processKeyboard(Direction direction, float deltaTime) {
    float velocity = speed * deltaTime;
    
    switch (direction) {
        case FORWARD:
            position += forwardVec * velocity;
            break;
        case BACKWARD:
            position -= forwardVec * velocity;
            break;
        case LEFT:
            position -= rightVec * velocity;
            break;
        case RIGHT:
            position += rightVec * velocity;
            break;
        default:
            break;
    }
}

void FPSCamera::jump() {
    if (!isJumping) {
        isJumping = true;
        velocity = jumpHeight; // Set initial jump velocity
    }
}

void FPSCamera::updatePhysics(float deltaTime) {
    if (isJumping) {
        position.y += velocity * deltaTime; // Move up/down based on velocity
        velocity += gravity * deltaTime; // Apply gravity
        
        // Check if the camera has landed
        if (position.y <= 0.0f) { // Ground level
            position.y = 0.0f; // Reset to ground level
            isJumping = false; // Reset jumping status
            velocity = 0.0f; // Reset vertical velocity
        }
    }
}

glm::mat4 FPSCamera::getViewMatrix() const {
    return glm::lookAt(position, position + forwardVec, upVec);
}

std::array<glm::vec4, 6> FPSCamera::getFrustumPlanes(const glm::mat4& projection) const {
    // Placeholder for frustum plane calculations (to be implemented)
    std::array<glm::vec4, 6> planes;
    
    return planes;
}
