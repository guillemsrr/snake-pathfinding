// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Pathfinding/Algorithms/AStarPathfinder.h"
#include "Pathfinding/Algorithms/AStarPathfinder.h"

class Camera
{
public:
    Camera(glm::vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& pos);
    void SetTarget(const glm::vec3& target);
    void SetAspectRatio(float aspectRatio);

    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;

    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;

private:
    glm::vec3 _position;
    glm::vec3 _target;
    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;

    glm::vec3 _upVector = { 0.f, 1.f, 0.f };

    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float fov, float aspect, float nearP, float farP);
};