//
// Created by user on 10/27/2025.
//

#include "SceneManager.h"

SceneManager::SceneManager() {

}

void SceneManager::AddScene(Scene *scene) {
    scenes.push_back(scene);
}

Scene* SceneManager::getCurrentScene() const{
    return scenes[currentScene];
}

void SceneManager::NextScene() {
    currentScene = (currentScene + 1) % scenes.size();
}

void SceneManager::PreviousScene() {
    currentScene--;
    if (currentScene < 0) {
        currentScene = scenes.size() - 1;
    }
}


