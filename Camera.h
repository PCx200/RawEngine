//
// Created by user on 10/28/2025.
//

#ifndef RAWENGINE_CAMERA_H
#define RAWENGINE_CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Transform.h"


class Camera {
public:
    Transform transform;

    float fov = 45.0f;
    float aspect = 16.0f/ 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    Camera();

    Camera(float x, float y, float z);

    Camera(float fov, float aspect, float nearPlane, float farPlane);

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    void ProcessMovementInput(GLFWwindow *window, float deltaTime, float speed);

    void ProcessMouseInput(GLFWwindow *window, float deltaTime, float speed);


};


#endif //RAWENGINE_CAMERA_H