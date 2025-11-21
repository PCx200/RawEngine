//
// Created by user on 11/17/2025.
//

#ifndef RAWENGINE_LIGHT_H
#define RAWENGINE_LIGHT_H
#include "Transform.h"


class Light {
private: glm::vec4 color;

public:
    Transform transform;

    Light(float x, float y, float z,glm::vec4 color);

    ~Light();

    Light& operator = (const Light & other);

    glm::vec4 getColor() const;
};


#endif //RAWENGINE_LIGHT_H