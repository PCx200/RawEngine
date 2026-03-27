//
// Created by ACER on 24.3.2026 г..
//

#include "CubeCollider.h"
#include <glad/glad.h>
#include "Shader.h"

GLuint CubeCollider::VAO = 0;
GLuint CubeCollider::VBO = 0;
GLuint CubeCollider::EBO = 0;
bool CubeCollider::initialised = false;

CubeCollider::CubeCollider(GLuint shaderProgramID) : shaderProgramID(shaderProgramID)
{
    if (!initialised)
        init();
}

CubeCollider::CubeCollider(glm::vec3 position, GLuint shaderProgramID) : shaderProgramID(shaderProgramID)
{
    transform.position = position;

    if (!initialised)
        init();
}

CubeCollider::CubeCollider(glm::vec3 position, glm::vec3 rotation, GLuint shaderProgramID) : shaderProgramID(shaderProgramID)
{
    transform.position = position;
    transform.rotation = rotation;

    if (!initialised)
        init();
}

CubeCollider::CubeCollider(glm::vec3 position, glm::vec3 rotation, float size, GLuint shaderProgramID) : shaderProgramID(shaderProgramID)
{
    transform.position = position;
    transform.rotation = rotation;

    transform.scale.x = size;
    transform.scale.y = size;
    transform.scale.z = size;

    if (!initialised)
        init();
}

void CubeCollider::init()
{
    initialised = true;

    float vertices[]
    {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    unsigned int indices[]
    {
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        0, 4,
        4, 5,
        5, 1,

        5, 6,
        6, 2,

        6, 7,
        7, 3,

        7, 4,
        4, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

bool CubeCollider::is_colliding(CubeCollider& other)
{
    glm::vec3 center = transform.position;
    glm::vec3 halfSize = transform.scale / 2.0f;

    //implement naive collision detections

    //use octree for optimisation

    return true;
}

OBB CubeCollider::get_OBB() const
{
    OBB obb{};

    obb.center = transform.position;
    obb.halfSize = transform.scale / 2.0f;

    obb.axes[0] = glm::normalize(glm::vec3(transform.rotation.x));
    obb.axes[1] = glm::normalize(glm::vec3(transform.rotation.y));
    obb.axes[2] = glm::normalize(glm::vec3(transform.rotation.z));

    return obb;
}

void CubeCollider::Render(const glm::mat4& viewProj)
{
    //glUseProgram(shaderProgramID);

    glm::mat4 model = transform.getMatrix();
    glm::mat4 mvp = viewProj * model;

    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

}
