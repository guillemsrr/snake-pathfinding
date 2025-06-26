// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane);

    float _yaw = 0.f;
    float _pitch = 0.f;
    float _radius = 30.f;

    void SetTarget(const glm::vec3& target);
    void SetAspectRatio(float aspectRatio);

    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;

    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    void UpdatePosition();
    void ApplyMotion(float xrel, float yrel);
    void AddRadius(float x);

private:
    glm::vec3 _position;
    glm::vec3 _target;
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;

    glm::vec3 _upVector = {0.f, 1.f, 0.f};

    const float _sensitivity = 0.005f;
    const float _zoomSensitivity = 0.5f;
    const float _pitchLimit = glm::radians(89.0f);

    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float fov, float aspect, float nearP, float farP);
};