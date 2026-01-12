//
// Created by user on 10/27/2025.
//

#ifndef RAWENGINE_GAMEOBJECT_H
#define RAWENGINE_GAMEOBJECT_H
#include "Transform.h"
#include "core/model.h"
#include "core/texture.h"


class GameObject {
public:
    Transform transform;

    core::Model model;
    core::Texture texture;
    const GLuint shaderProgramID;

    GameObject(const core::Model& model, GLuint shaderProgram);

    GameObject(const core::Model& model, core::Texture& texture, GLuint shaderProgram);

    void Update(float deltaTime);

    void Render(const glm::mat4& viewProj);

};


#endif //RAWENGINE_GAMEOBJECT_H