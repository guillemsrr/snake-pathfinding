#include "Camera.h"

Camera::Camera(glm::vec3 position, float fov, float aspect, float nearP, float farP)
    : _position(position), _target(0.0f, 0.0f, 0.0f)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix(fov, aspect, nearP, farP);
}

void Camera::SetPosition(const glm::vec3& pos)
{
    _position = pos;
    UpdateViewMatrix();
}

void Camera::SetTarget(const glm::vec3& target)
{
    _target = target;
    UpdateViewMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
    UpdateProjectionMatrix(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

const glm::mat4& Camera::GetViewMatrix() const
{
    return _viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return _projectionMatrix;
}

glm::vec3 Camera::GetForward() const
{
    glm::vec3 forward = glm::normalize(_target - _position);
    return forward;
}

glm::vec3 Camera::GetRight() const
{
    glm::vec3 forward = glm::normalize(_target - _position);
    glm::vec3 right = glm::normalize(glm::cross(forward, _upVector));
    return right;
}

void Camera::UpdateViewMatrix()
{
    _viewMatrix = glm::lookAt(_position, _target, _upVector);
}

void Camera::UpdateProjectionMatrix(float fov, float aspect, float nearP, float farP)
{
    _projectionMatrix = glm::perspective(fov, aspect, nearP, farP);
}