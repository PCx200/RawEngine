//
// Created by user on 10/27/2025.
//

#ifndef RAWENGINE_TRANSFORM_H
#define RAWENGINE_TRANSFORM_H

#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform();

    ~Transform();

    glm::mat4 getMatrix() const;

    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    void Translate(const glm::vec3& translation);
    void Rotate(const glm::vec3& axis, float angleDeg);
};


#endif //RAWENGINE_TRANSFORM_H