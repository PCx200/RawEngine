//
// Created by user on 10/27/2025.
//

#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>

Transform::Transform() : position(0.0f), rotation(0.0), scale(1.0f) {}

Transform::~Transform() = default;



glm::mat4 Transform::getMatrix() const {

    glm::mat4 model(1.0f);

    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // yaw
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // pitch
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // roll

    model = glm::scale(model, scale);

    return model;
}

glm::vec3 Transform::forward() const {
    glm::vec3 forward;
    forward.x = cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y));
    forward.y = sin(glm::radians(rotation.x));
    forward.z = -cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)); // minus is funny?

    // This is just for comparison: comment it out later:
    // auto mat = getMatrix();
    // auto fwd2 = mat * glm::vec4( 0,0,1,0 );
    //printf("forward: (%f,%f,%f)  from matrix: (%f,%f,%f)\n",forward.x,forward.y,forward.z, fwd2.x, fwd2.y, fwd2.z);

    return forward;
}
glm::vec3 Transform::right() const {
    return glm::normalize(cross(forward(), glm::vec3(0, 1, 0)));
}
glm::vec3 Transform::up() const {
    return glm::normalize(cross(right(), forward()));
}

void Transform::Translate(const glm::vec3& translation) {
    position += translation;
}

void Transform::Rotate(const glm::vec3& axis, float angleDeg) {
    rotation+= glm::normalize(axis) * angleDeg;
}
