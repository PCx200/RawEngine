//
// Created by ACER on 17.4.2026 г..
//

#include "Octree.h"

Octree::Octree(glm::vec3 center, float half_width, int max_depth, Shader shader)
{
    root = build(center, half_width, max_depth, shader);
}

Node* Octree::build(glm::vec3 center, float half_width, int max_depth, Shader shader)
{
    if (max_depth < 0) return nullptr;

    Node* node = new Node;
    node->center = center;
    node->half_width = half_width;

    glm::vec3 offset;
    float step = half_width * 0.5f;

    for (int i = 0; i < 8; i++)
    {
        offset.x = ((i & 1) ? step : -step);
        offset.y = ((i & 2) ? step : -step);
        offset.z = ((i & 4) ? step : -step);
        node->child[i] = build(center + offset, step, max_depth - 1, shader);
        node->collider = new CubeCollider(center, glm::vec3(0,0,0), half_width * 2, shader);
    }

    return node;
}

void Octree::insert(Node* node, CubeCollider* collider)
{
    int index = 0;
    bool straddle = false;

    for (int i = 0; i < 3; i++) {
        float delta = collider->transform.position[i] - node->center[i];
        if (fabs(delta) < node->half_width + collider->size) {
            straddle = true;
            break;
        }
        if (delta > 0.0f) index |= (1 << i); // ZYX
    }
    if (!straddle && node->child[index]) {
        insert(node->child[index], collider);
    } else {
        node->colliders.push_back(collider);
    }
}

void Octree::check_collisions(Node* node)
{
    for (size_t i = 0; i < node->colliders.size(); i++)
    {
        for (size_t j = i + 1; j < node->colliders.size(); j++)
        {
            if (node->colliders[i]->intersects(*node->colliders[j]))
            {
                node->colliders[i]->is_intersecting = true;
                node->colliders[j]->is_intersecting = true;
            }
        }
    }

    for (int c = 0; c < 8; c++)
    {
        Node* child = node->child[c];
        if (child)
        {
            check_collisions(child);
        }
    }
}

void Octree::clear(Node* node)
{
    node->colliders.clear();

    for (int c = 0; c < 8; c++)
    {
        Node* child = node->child[c];
        if (!child) continue;

        clear(child);
    }
}

void Octree::render(Node* node, glm::mat4 view_proj)
{
    for (auto n : node->child)
    {
        if (!n) continue;
        n->collider->Render(view_proj);
        render(n, view_proj);
    }
}
