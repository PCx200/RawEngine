//
// Created by user on 10/27/2025.
//

#include <vector>
#include "Scene.h"

#ifndef RAWENGINE_SCENEMANAGER_H
#define RAWENGINE_SCENEMANAGER_H


class SceneManager {
private:
    std::vector<Scene*> scenes;
    int currentScene = 0;
public:
    SceneManager();

    void AddScene(Scene* scene);

    void NextScene();

    void PreviousScene();

    Scene* getCurrentScene() const;



};


#endif //RAWENGINE_SCENEMANAGER_H