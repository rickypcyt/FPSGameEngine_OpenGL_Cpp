#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <array>

class FPSCamera {
public:
    // Constructor
    FPSCamera(const glm::vec3& pos = glm::vec3(0.0f), float pitchDeg = 0.0f, float yawDeg = -90.0f);

    // Update camera vectors based on yaw and pitch
    void updateVectors();

    // Process keyboard input for movement
    enum Direction { FORWARD, BACKWARD, LEFT, RIGHT };
    void processKeyboard(Direction direction, float deltaTime);

    // Jump functionality
    void jump();

    // Update physics for jumping and gravity
    void updatePhysics(float deltaTime);

    // Get the view matrix for rendering
    glm::mat4 getViewMatrix() const;

    // Calculate frustum planes for visibility
    std::array<glm::vec4, 6> getFrustumPlanes(const glm::mat4& projection) const;

private:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 forwardVec;
    glm::vec3 rightVec;
    glm::vec3 upVec;
    glm::vec3 worldUp;

    float pitch;
    float yaw;

    // Constants for pitch limits
    const float MIN_PITCH;
    const float MAX_PITCH;

    // Movement speed and jump settings
    float speed;
    float jumpHeight;
    float gravity;

    // Jump state
    bool isJumping;
    float velocity; // Current vertical velocity
};

#endif // CAMERA_H
