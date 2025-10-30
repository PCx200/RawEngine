//
// Created by user on 10/27/2025.
//

#include "Scene.h"

Scene::Scene(std::string name,Camera *camera) : name(name) ,camera(camera) {
}

void Scene::AddObject(GameObject *object) {
    objects.push_back(object);
}

void Scene::Update(float deltaTime) {
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Update(deltaTime);
    }
}


void Scene::Render() const{
    glm::mat4 VP = camera->getProjectionMatrix() * camera->getViewMatrix();
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Render(VP);
    }
}

