//
// Created by user on 11/17/2025.
//

#include "Light.h"

#include "GameObject.h"
#include "core/assimpLoader.h"
#include "core/model.h"

Light::Light(glm::vec3 position, glm::vec4 color, float radius, GLuint shaderProgram) : GameObject(core::AssimpLoader::loadModel("models/sphere.obj"), shaderProgram),  color(color) {
    initialize(position, radius);
}

Light::Light(float x, float y, float z, glm::vec4 color, float radius, GLuint shaderProgram) : GameObject(core::AssimpLoader::loadModel("models/sphere.obj"), shaderProgram),  color(color) {
    initialize(glm::vec3(x,y,z), radius);
}

void Light::initialize(glm::vec3 position, float radius) {
    transform.position = position;
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

void Light::setColor(glm::vec4 newColor) {
    this->color = newColor;
}




