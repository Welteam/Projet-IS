#include "Scene.h"
#include <iostream>
#include "state/TileType.h"


namespace render {

    Scene::Scene(sf::RenderWindow &window, sf::View &view) : window(window), view(view) {

    }

    void Scene::updateWorld(state::World world) {
        if(isWindowAvailable(window)){
            LayerRender map;
            std::vector<state::TileType> input = world.getTiles();
            int arr[input.size()];
            std::copy(input.begin(), input.end(), arr);
            if (!map.load("../res/tiles2.png", sf::Vector2u(16, 16), arr, world.getXMax(), world.getYMax()))
                std::cout << "Cannot load map" << std::endl;
            this->worldRender = map;
            updateAll();
        }
    }

    void Scene::updatePlayers(state::Player player) {
        if(isWindowAvailable(window)){
            // on charge la texture du tileset
            if (!unitsRed.loadFromFile("../res/units_red.png"))
                std::cout << "Cannot load Texture : ../res/units_red.png" << std::endl;
            if (!unitsBlue.loadFromFile("../res/units_blue.png"))
                std::cout << "Cannot load Texture : ../res/units_blue.png" << std::endl;
            std::vector<sf::Sprite> sprites;

            for(state::Character character :  player.getUnits()){
                sf::Sprite unit;
                if(player.getId()== 1) {
                    unit.setTexture(unitsRed);
                } else {
                    unit.setTexture(unitsBlue);
                }
                unit.setTextureRect(sf::IntRect(0, 0, 16, 16));
                unit.setPosition(sf::Vector2f(character.getX()*320/20, character.getY()*320/20)); // position absolue
                sprites.push_back(unit);
            }
            for(state::Tower towerFromPlayer :  player.getTowers()){
                sf::Sprite tower;
                if(player.getId()== 1) {
                    tower.setTexture(unitsRed);
                } else {
                    tower.setTexture(unitsBlue);
                }
                tower.setTextureRect(sf::IntRect(0, 64, 16, 16));
                tower.setPosition(sf::Vector2f(towerFromPlayer.getX() * 320 / 20, towerFromPlayer.getY() * 320 / 20)); // position absolue
                sprites.push_back(tower);
            }
            for(state::ApparitionArea apparitionAreaFromPlayer :  player.getApparitionAreas()){
                sf::Sprite apparitionArea;
                if(player.getId()== 1) {
                    apparitionArea.setTexture(unitsRed);
                } else {
                    apparitionArea.setTexture(unitsBlue);
                }
                apparitionArea.setTextureRect(sf::IntRect(16, 64, 16, 16));
                apparitionArea.setPosition(sf::Vector2f(apparitionAreaFromPlayer.getX() * 320 / 20, apparitionAreaFromPlayer.getY() * 320 / 20)); // position absolue
                sprites.push_back(apparitionArea);
            }
            if(player.getId() == 1) {
                this->player1Render = sprites;
            }
            else {
                this->player2Render = sprites;
            }
            updateAll();
        }
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

    void Scene::updateAll() {
        if(isWindowAvailable(window)){
            window.clear();
            float xView;
            float yView;
            float widthView;
            float heightView;
            if(window.getSize().x > window.getSize().y){
                xView = 0.5f*(1.f-(1.f*window.getSize().y)/(1.0f*window.getSize().x));
                yView = 0.0f;
                widthView = (1.f*window.getSize().y)/(1.0f*window.getSize().x);
                heightView = 1.0f;
            } else if(window.getSize().x < window.getSize().y){
                xView = 0.0f;
                yView = 0.5f*(1.f-(1.f*window.getSize().x)/(1.f*window.getSize().y));
                widthView = 1.0f;
                heightView = (1.f*window.getSize().x)/(1.f*window.getSize().y);
            } else {
                xView = 0.0f;
                yView = 0.0f;
                widthView = 1.0f;
                heightView = 1.0f;
            }
            view.setViewport(sf::FloatRect(xView, yView, widthView, heightView));
            window.setView(view);
            window.draw(this->worldRender);
            for(auto sprite : this->player1Render)
                window.draw(sprite);
            for(auto sprite : this->player2Render)
                window.draw(sprite);
            window.display();
        }
    }

}