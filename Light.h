//
// Created by user on 11/17/2025.
//

#ifndef RAWENGINE_LIGHT_H
#define RAWENGINE_LIGHT_H
#include "GameObject.h"
#include "Transform.h"


class Light : public GameObject {
private: glm::vec4 color;

public:
    //Transform transform;

    float radius;
    Light(glm::vec3 position,glm::vec4 color, float radius, GLuint shaderProgram);
    Light(float x, float y, float z,glm::vec4 color, float radius, GLuint shaderProgram);

    ~Light();

    Light& operator = (const Light & other);

    glm::vec4 getColor() const;

    void setColor(glm::vec4 newColor);

private:
    void initialize(glm::vec3 position, float radius);
};


#endif //RAWENGINE_LIGHT_H