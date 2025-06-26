#include "Camera.h"

Camera::Camera(glm::vec3 position, float fov, float aspect, float nearP, float farP)
    : _position(position), _target(0.0f, 0.0f, 0.0f)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix(fov, aspect, nearP, farP);
}

void Camera::SetTarget(const glm::vec3& target)
{
    _target = target;
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

void Camera::UpdatePosition()
{
    float camX = _radius * cosf(_pitch) * sinf(_yaw);
    float camY = _radius * sinf(_pitch);
    float camZ = _radius * cosf(_pitch) * cosf(_yaw);
    _position = _target + glm::vec3(camX, camY, camZ);

    UpdateViewMatrix();
}

void Camera::ApplyMotion(float xrel, float yrel)
{
    _yaw -=  xrel* _sensitivity;
    _pitch -= yrel * _sensitivity;

    _pitch = glm::clamp(_pitch, -_pitchLimit, _pitchLimit);
}

void Camera::AddRadius(float wheelValue)
{
    _radius -=  wheelValue * _zoomSensitivity;
    _radius = glm::clamp(_radius, 5.0f, 100.0f);
}

void Camera::UpdateViewMatrix()
{
    _viewMatrix = glm::lookAt(_position, _target, _upVector);
}

void Camera::UpdateProjectionMatrix(float fov, float aspect, float nearP, float farP)
{
    _projectionMatrix = glm::perspective(fov, aspect, nearP, farP);
}