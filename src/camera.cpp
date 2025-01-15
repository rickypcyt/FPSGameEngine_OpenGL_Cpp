#include "../include/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float PI = 3.14159265358979323846f;

FPSCamera::FPSCamera(const glm::vec3& pos, float pitchDeg, float yawDeg)
    : position(pos),
      pitch(glm::radians(pitchDeg)),
      yaw(glm::radians(yawDeg)),
      MIN_PITCH(-89.0f * PI / 180.0f),
      MAX_PITCH(89.0f * PI / 180.0f),
      worldUp(0.0f, 1.0f, 0.0f),
      speed(2.5f), // Movement speed
      jumpHeight(5.0f), // Jump height
      gravity(-9.81f), // Gravity
      isJumping(false),
      velocity(0.0f)
{
    updateVectors();
}

void FPSCamera::updateVectors() {
    pitch = glm::clamp(pitch, MIN_PITCH, MAX_PITCH);
    forwardVec.x = cos(yaw) * cos(pitch);
    forwardVec.y = sin(pitch);
    forwardVec.z = sin(yaw) * cos(pitch);
    forwardVec = glm::normalize(forwardVec);
    rightVec = glm::normalize(glm::cross(forwardVec, worldUp));
    upVec = glm::normalize(glm::cross(rightVec, forwardVec));
}

void FPSCamera::processKeyboard(Direction direction, float deltaTime) {
    float velocity = speed * deltaTime;
    if (direction == FORWARD)
        position += forwardVec * velocity;
    if (direction == BACKWARD)
        position -= forwardVec * velocity;
    if (direction == LEFT)
        position -= rightVec * velocity;
    if (direction == RIGHT)
        position += rightVec * velocity;
}

void FPSCamera::jump() {
    if (!isJumping) {
        isJumping = true;
        velocity = jumpHeight;
    }
}

void FPSCamera::updatePhysics(float deltaTime) {
    if (isJumping) {
        position.y += velocity * deltaTime;
        velocity += gravity * deltaTime; // Apply gravity
        if (position.y <= 0.0f) { // Ground level
            position.y = 0.0f; // Reset to ground
            isJumping = false; // Reset jumping status
        }
    }
}

glm::mat4 FPSCamera::getViewMatrix() const {
    return glm::lookAt(position, position + forwardVec, upVec);
}
