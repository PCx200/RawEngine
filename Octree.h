//
// Created by ACER on 17.4.2026 г..
//

#ifndef RAWENGINE_OCTREE_H
#define RAWENGINE_OCTREE_H
#include "CubeCollider.h"
#include <vector>
#include <glm/vec3.hpp>

struct Node
{
    glm::vec3 center;
    float half_width;
    float loose_half_width;
    Node* child[8]{};
    std::vector<CubeCollider*> colliders;
    CubeCollider* collider = nullptr;
};

class Octree
{
public:
    int octree_checks;
    int octree_collisions;
    Node* root;

    Octree(glm::vec3 center, float half_width, int max_depth, Shader shader);

    Node* build(glm::vec3 center, float half_width, int max_depth, Shader shader);
    void check_collisions(Node* node);
    void clear(Node* node);
    void insert(Node* node, CubeCollider* collider);
    void loose_insert(Node* node, CubeCollider* collider);

    void render(Node* node, glm::mat4 view_proj);

private:

};


#endif //RAWENGINE_OCTREE_H