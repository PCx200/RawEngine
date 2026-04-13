//
// Created by ACER on 24.3.2026
//

#ifndef RAWENGINE_CUBECOLLIDER_H
#define RAWENGINE_CUBECOLLIDER_H
#include <glad/glad.h>
#include "Transform.h"

struct OBB
{
    glm::vec3 center;
    glm::vec3 halfSize;
    glm::vec3 axes[3];
};

class CubeCollider
{
    private:
        static GLuint VAO, VBO, EBO;
        static bool initialised;

    public:
        const GLuint shaderProgramID;
        bool is_intersecting;
        float size;
        Transform transform;

        CubeCollider(GLuint shaderProgramID);
        CubeCollider(glm::vec3 position, GLuint shaderProgramID);
        CubeCollider(glm::vec3 position, glm::vec3 rotation, GLuint shaderProgramID);
        CubeCollider(glm::vec3 position, glm::vec3 rotation, float size, GLuint shaderProgramID);

        static void init();

        OBB get_OBB() const;
        bool intersects(const CubeCollider& other);
        void Render(const glm::mat4& viewProj);
};


#endif //RAWENGINE_CUBECOLLIDER_H