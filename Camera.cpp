//
// Created by user on 10/28/2025.
//

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() = default;

Camera::Camera(float x, float y, float z) {
    transform.position = glm::vec3(x,y,z);
}

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

void Camera::ProcessMovementInput(GLFWwindow *window, float deltaTime, float speed) {

    glm::vec3 direction(0.0f);

    //forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction += transform.forward();
    //backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction -= transform.forward();
    //right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction += transform.right();
    //left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction -= transform.right();
    //up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        direction += transform.up();
    //down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        direction -= transform.up();

    if (glm::length(direction) > 0.0f)
        direction = glm::normalize(direction);
    transform.Translate(direction * speed * deltaTime);
}

void Camera::ProcessMouseInput(GLFWwindow *window, float deltaTime, float speed) {
    
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        transform.rotation.y += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        transform.rotation.y -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        transform.rotation.x += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        transform.rotation.x -= speed * deltaTime;

    if (transform.rotation.x > 89.0f)
        transform.rotation.x = 89.0f;
    if (transform.rotation.x < -89.0f)
        transform.rotation.x = -89.0f;
}


