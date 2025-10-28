//
// Created by user on 10/27/2025.
//

#include "GameObject.h"

#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(const core::Model &model, GLuint shaderProgram) : model(model), shaderProgramID(shaderProgram) {}

void GameObject::Render(const glm::mat4 &viewProj) {
    glUseProgram(shaderProgramID);
    glm::mat4 mvpMatrix = viewProj * transform.getMatrix();
    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvpMatrix");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    model.render();
    glBindVertexArray(0);
}


