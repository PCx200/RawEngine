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

OBB CubeCollider::get_OBB() const
{
    OBB obb{};

    obb.center = transform.position;
    obb.halfSize = transform.scale / 2.0f;

    glm::mat4 M = transform.getMatrix();
    glm::mat3 R = glm::mat3(M);

    obb.axes[0] = glm::normalize(glm::vec3(R[0]));
    obb.axes[1] = glm::normalize(glm::vec3(R[1]));
    obb.axes[2] = glm::normalize(glm::vec3(R[2]));

    return obb;
}

bool CubeCollider::intersects(const CubeCollider& other)
{
    OBB A = get_OBB();
    OBB B = other.get_OBB();

    float rot_mat_B_A[3][3];
    float abs_rot_mat_B_A[3][3];


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            rot_mat_B_A[i][j] = glm::dot(A.axes[i], B.axes[j]);
        }
    }

    glm::vec3 distance = B.center - A.center;
    distance = glm::vec3(glm::dot(distance, A.axes[0]),
                            glm::dot(distance, A.axes[1]),
                            glm::dot(distance, A.axes[2]));

    const float epsilon = 1e-5f;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            abs_rot_mat_B_A[i][j] = std::fabs(rot_mat_B_A[i][j]) + epsilon;
        }
    }

    float extentA, extentB;

    //A face normals
    for (int i = 0; i < 3; i++)
    {
        extentA = A.halfSize[i];
        extentB = B.halfSize.x * abs_rot_mat_B_A[i][0] +
                  B.halfSize.y * abs_rot_mat_B_A[i][1] +
                  B.halfSize.z * abs_rot_mat_B_A[i][2];

        if (std::fabs(distance[i]) > extentA + extentB) return false;
    }

    //B face normals
    for (int j = 0; j < 3; j++)
    {
        extentB = B.halfSize[j];

        extentA = A.halfSize.x * abs_rot_mat_B_A[0][j] +
                  A.halfSize.y * abs_rot_mat_B_A[1][j] +
                  A.halfSize.z * abs_rot_mat_B_A[2][j];

        float projection = std::fabs(distance.x * rot_mat_B_A[0][j] +
                                        distance.y * rot_mat_B_A[1][j] +
                                        distance.z * rot_mat_B_A[2][j]);

        if (projection > extentA + extentB) return false;
    }

    //9 cross products
    extentA = A.halfSize.y * abs_rot_mat_B_A[2][0] + A.halfSize.z * abs_rot_mat_B_A[1][0];
    extentB = B.halfSize.y * abs_rot_mat_B_A[0][2] + B.halfSize.z * abs_rot_mat_B_A[0][1];
    if (std::fabs(distance.z * rot_mat_B_A[1][0] - distance.y * rot_mat_B_A[2][0]) > extentA + extentB) return false;

    extentA = A.halfSize.y * abs_rot_mat_B_A[2][1] + A.halfSize.z * abs_rot_mat_B_A[1][1];
    extentB = B.halfSize.x * abs_rot_mat_B_A[0][2] + B.halfSize.z * abs_rot_mat_B_A[0][0];
    if (std::fabs(distance.z * rot_mat_B_A[1][1] - distance.y * rot_mat_B_A[2][1]) > extentA + extentB) return false;

    extentA = A.halfSize.y * abs_rot_mat_B_A[2][2] + A.halfSize.z * abs_rot_mat_B_A[1][2];
    extentB = B.halfSize.x * abs_rot_mat_B_A[0][1] + B.halfSize.y * abs_rot_mat_B_A[0][0];
    if (std::fabs(distance.z * rot_mat_B_A[1][2] - distance.y * rot_mat_B_A[2][2]) > extentA + extentB) return false;



    extentA = A.halfSize.x * abs_rot_mat_B_A[2][0] + A.halfSize.z * abs_rot_mat_B_A[0][0];
    extentB = B.halfSize.y * abs_rot_mat_B_A[1][2] + B.halfSize.z * abs_rot_mat_B_A[1][1];
    if (std::fabs(distance.x * rot_mat_B_A[2][0] - distance.z * rot_mat_B_A[0][0]) > extentA + extentB) return false;

    extentA = A.halfSize.x * abs_rot_mat_B_A[2][1] + A.halfSize.z * abs_rot_mat_B_A[0][1];
    extentB = B.halfSize.x * abs_rot_mat_B_A[1][2] + B.halfSize.z * abs_rot_mat_B_A[1][0];
    if (std::fabs(distance.x * rot_mat_B_A[2][1] - distance.z * rot_mat_B_A[0][1]) > extentA + extentB) return false;

    extentA = A.halfSize.x * abs_rot_mat_B_A[2][2] + A.halfSize.z * abs_rot_mat_B_A[0][2];
    extentB = B.halfSize.x * abs_rot_mat_B_A[1][1] + B.halfSize.y * abs_rot_mat_B_A[1][0];
    if (std::fabs(distance.x * rot_mat_B_A[2][2] - distance.z * rot_mat_B_A[0][2]) > extentA + extentB) return false;



    extentA = A.halfSize.x * abs_rot_mat_B_A[1][0] + A.halfSize.y * abs_rot_mat_B_A[0][0];
    extentB = B.halfSize.y * abs_rot_mat_B_A[2][2] + B.halfSize.z * abs_rot_mat_B_A[2][1];
    if (std::fabs(distance.y * rot_mat_B_A[0][0] - distance.x * rot_mat_B_A[1][0]) > extentA + extentB) return false;

    extentA = A.halfSize.x * abs_rot_mat_B_A[1][1] + A.halfSize.y * abs_rot_mat_B_A[0][1];
    extentB = B.halfSize.x * abs_rot_mat_B_A[2][2] + B.halfSize.z * abs_rot_mat_B_A[2][0];
    if (std::fabs(distance.y * rot_mat_B_A[0][1] - distance.x * rot_mat_B_A[1][1]) > extentA + extentB) return false;

    extentA = A.halfSize.x * abs_rot_mat_B_A[1][2] + A.halfSize.y * abs_rot_mat_B_A[0][2];
    extentB = B.halfSize.x * abs_rot_mat_B_A[2][1] + B.halfSize.y * abs_rot_mat_B_A[2][0];
    if (std::fabs(distance.y * rot_mat_B_A[0][2] - distance.x * rot_mat_B_A[1][2]) > extentA + extentB) return false;

    return true;
}

void CubeCollider::Render(const glm::mat4& viewProj)
{
    //glUseProgram(shaderProgramID);
    glUniform1i(glGetUniformLocation(shaderProgramID, "intersects"), is_intersecting ? 1 : 0);

    glm::mat4 model = transform.getMatrix();
    glm::mat4 mvp = viewProj * model;

    GLuint mvpLoc = glGetUniformLocation(shaderProgramID, "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

}
