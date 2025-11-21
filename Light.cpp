//
// Created by user on 11/17/2025.
//

#include "Light.h"

Light::Light(float x, float y, float z, glm::vec4 color) : color(color) {
    transform.position = glm::vec3(x,y,z);
}

Light::~Light() = default;

Light &Light::operator=(const Light &other) {
    if (this != &other) {
        this->transform = other.transform;
        this->color = other.color;
    }
    return *this;
}

glm::vec4 Light::getColor() const {
    return this->color;
}




