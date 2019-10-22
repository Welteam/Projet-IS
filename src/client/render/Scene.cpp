#include "Scene.h"
#include <iostream>

namespace render {

    Scene::Scene(sf::RenderWindow &window) {

    }

    void Scene::draw(sf::RenderWindow &window) {

    }

    void Scene::updateWorld() {

    }

    void Scene::updatePlayers() {

    }

    void Scene::stateChanged (const state::StateEvent &e, state::GameState &gameState){
        std::cout<< "Hey je suis Scene Observer: " << std::endl;
    }



}