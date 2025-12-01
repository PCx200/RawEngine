//
// Created by user on 10/27/2025.
//


#ifndef RAWENGINE_SCENE_H
#define RAWENGINE_SCENE_H
#include <vector>


#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include "core/model.h"


class Scene {
private:
    std::vector<GameObject*> objects;
    Camera* camera;

    std::vector<Light*> lights;

public:
    std::string name;

    Scene(std::string name,Camera* camera);

    ~Scene();

    // Scene& operator = (const Scene& other);

    void AddObject(GameObject* object);

    void AddLight(Light* light);

    int GetLightCount();

    void Update(float deltaTime);

    void Render() const;

};


#endif //RAWENGINE_SCENE_H