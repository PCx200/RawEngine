//
// Created by user on 10/27/2025.
//

#include "GameObject.h"

#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(const core::Model &model, GLuint shaderProgram) : model(model), texture(core::Texture("textures/default_texture.png")), shaderProgramID(shaderProgram) {}

GameObject::GameObject(const core::Model& model, core::Texture& texture, GLuint shaderProgram): model(model), texture(texture), shaderProgramID(shaderProgram) {}

void GameObject::Update(float deltaTime) {
    //transform.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 50 * deltaTime);
}

void GameObject::Render(const glm::mat4 &viewProj) {
    glUseProgram(shaderProgramID);

    glBindTexture(GL_TEXTURE_2D, texture.getId());
    glBindVertexArray(0);

    glm::mat4 mvpMatrix = viewProj * transform.getMatrix();
    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvpMatrix");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    GLint modelMatrixLoc = glGetUniformLocation(shaderProgramID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(transform.getMatrix()));
    model.render();
    glBindVertexArray(0);
}


