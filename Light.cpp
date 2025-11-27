//
// Created by user on 11/17/2025.
//

#include "Light.h"

Light::Light(float x, float y, float z, glm::vec4 color, float radius) : color(color) {
    transform.position = glm::vec3(x,y,z);
    this->radius = radius;
}

Light::~Light() = default;

Light &Light::operator=(const Light &other) {
    if (this != &other) {
        this->transform = other.transform;
        this->color = other.color;
        this->radius = other.radius;
    }
    return *this;
}

glm::vec4 Light::getColor() const {
    return this->color;
}




