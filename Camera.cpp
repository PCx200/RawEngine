//
// Created by user on 10/28/2025.
//

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() = default;

Camera::Camera(float fov, float aspect, float nearPlane, float farPlane) {
    this->fov = fov;
    this->aspect = aspect;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
};

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(transform.position, transform.position + transform.forward(), transform.up());
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}


