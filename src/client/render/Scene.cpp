#include "Scene.h"
#include <iostream>
#include "state/TileType.h"


namespace render {

    Scene::Scene(sf::RenderWindow &window) : window(window) {

    }

    void Scene::updateWorld(state::World world) {
        if(isWindowAvailable(window)){
            LayerRender map;
            std::vector<state::TileType> input = world.getTiles();
            int arr[input.size()];
            std::copy(input.begin(), input.end(), arr);
            if (!map.load("res/tiles2.png", sf::Vector2u(16, 16), arr, world.getXMax(), world.getYMax()))
                std::cout << "Cannot load map" << std::endl;
            window.clear();
            window.draw(map);
            window.display();
        }
    }

    void Scene::updatePlayers() {

    }

    void Scene::stateChanged (const state::StateEvent &e, state::GameState &gameState){
        switch(e.stateEventID){
            case state::StateEventID::WORLD:
                this->updateWorld(gameState.getWorld());
                break;
            case state::StateEventID::PLAYER1:
                this->updatePlayers();
                break;
            case state::StateEventID::PLAYER2:
                this->updatePlayers();
                break;
            case state::StateEventID::ACTIVEPLAYER:
                // TODO Apply rendering coming from new active player
                break;
            case state::StateEventID::TURN:
                // TODO Update IHM who shows the number of turn
                break;
            default:
                std::cout << "Cannot read notification from gameState" << std::endl;
        }
    }

    bool Scene::isWindowAvailable(sf::RenderWindow &window) {
        if (&window == nullptr) {
            std::cout << "window reference in Scene is a null pointer" << std::endl;
            return false;
        }
        if (!window.isOpen()) {
            std::cout << "window reference in Scene is closed" << std::endl;
            return false;
        }
        return true;
    }

}