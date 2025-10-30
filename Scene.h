//
// Created by user on 10/27/2025.
//


#ifndef RAWENGINE_SCENE_H
#define RAWENGINE_SCENE_H
#include <vector>


#include "Camera.h"
#include "GameObject.h"
#include "core/model.h"


class Scene {
private:
    std::vector<GameObject*> objects;
    Camera* camera;
public:
    std::string name;

    Scene(std::string name,Camera* camera);

    void AddObject(GameObject* object);

    void Update(float deltaTime);

    void Render() const;

};


#endif //RAWENGINE_SCENE_H