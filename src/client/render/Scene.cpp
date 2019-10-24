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
            if (!map.load("../res/tiles2.png", sf::Vector2u(16, 16), arr, world.getXMax(), world.getYMax()))
                std::cout << "Cannot load map" << std::endl;
            window.clear();
            window.draw(map);
            window.display();
        }
    }

    void Scene::updatePlayers(state::Player player) {
        sf::Texture textureUnits;
        sf::Texture textureTowers;
        sf::Texture textureApparitionAreas;
        std::vector<sf::Sprite> sprites;
        if(player.getId() == 1) {
            if (!textureUnits.loadFromFile("../res/unity_red_right.png"))
                std::cout << "Cannot load Texture : ../res/unity_red_right.png" << std::endl;
            if (!textureTowers.loadFromFile("../res/tower_red.png"))
                std::cout << "Cannot load Texture : ../res/tower_red.png" << std::endl;
            if (!textureApparitionAreas.loadFromFile("../res/aa_red.png"))
                std::cout << "Cannot load Texture : ../res/aa_red.png" << std::endl;
        } else {
            if (!textureUnits.loadFromFile("../res/unity_blue_left.png"))
                std::cout << "Cannot load Texture : ../res/unity_blue_left.png" << std::endl;
            if (!textureTowers.loadFromFile("../res/tower_blue.png"))
                std::cout << "Cannot load Texture : ../res/tower_blue.png" << std::endl;
            if (!textureApparitionAreas.loadFromFile("../res/aa_blue.png"))
                std::cout << "Cannot load Texture : ../res/aa_blue.png" << std::endl;
        }
        for(state::Character character :  player.getUnits()){
            sf::Sprite unit;
            unit.setTexture(textureUnits);
            unit.setPosition(sf::Vector2f(character.getX()*320/20, character.getY()*320/20)); // position absolue
            sprites.push_back(unit);
        }
        for(state::Tower towerFromPlayer :  player.getTowers()){
            sf::Sprite tower;
            tower.setTexture(textureTowers);
            tower.setPosition(sf::Vector2f(towerFromPlayer.getX() * 320 / 20, towerFromPlayer.getY() * 320 / 20)); // position absolue
            sprites.push_back(tower);
        }
        for(state::ApparitionArea apparitionAreaFromPlayer :  player.getApparitionAreas()){
            sf::Sprite apparitionArea;
            apparitionArea.setTexture(textureApparitionAreas);
            apparitionArea.setPosition(sf::Vector2f(apparitionAreaFromPlayer.getX() * 320 / 20, apparitionAreaFromPlayer.getY() * 320 / 20)); // position absolue
            sprites.push_back(apparitionArea);
        }
        for(auto sprite : sprites)
            window.draw(sprite);
        window.display();


    }

    void Scene::stateChanged (const state::StateEvent &e, state::GameState &gameState){
        switch(e.getStateEventID()){
            case state::StateEventID::WORLD:
                std::cout << "Notification from gameState for WORLD" << std::endl;
                this->updateWorld(gameState.getWorld());
                break;
            case state::StateEventID::PLAYER1:
                std::cout << "Notification from gameState for PLAYER1" << std::endl;
                this->updatePlayers(gameState.getPlayer1());
                break;
            case state::StateEventID::PLAYER2:
                std::cout << "Notification from gameState for PLAYER2" << std::endl;
                this->updatePlayers(gameState.getPlayer2());
                break;
            case state::StateEventID::ACTIVEPLAYER:
                // TODO Apply rendering coming from new active player
                break;
            case state::StateEventID::TURN:
                // TODO Update IHM who shows the number of turn
                break;
            default:
                std::cout << "Cannot read notification from gameState : "<< e.getStateEventID() << std::endl;
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