#include "Scene.h"
#include <iostream>
#include "state/TileType.h"
#include "engine/DisplayAttack.cpp"

using namespace engine;
using namespace render;
using namespace state;
using namespace std;

Scene::Scene(sf::RenderWindow &window, sf::View &view) : window(window), view(view) {
    
}

void Scene::updateWorld(const World& world) {
    if(isWindowAvailable(window)){
        LayerRender map;
        vector<TileType> input = world.getTiles();
        int arr[input.size()];
        copy(input.begin(), input.end(), arr);
        if (!map.load("../res/tiles2.png", sf::Vector2u(16, 16), arr, world.getXMax(), world.getYMax()))
            cout << "Cannot load map" << endl;
        this->worldRender = map;
        updateAll();
    }
}
void Scene::updatePlayers(const Player& player) {
    if(isWindowAvailable(window)){
        // on charge la texture du tileset
        if (!unitsRed.loadFromFile("../res/units_red.png"))
            cout << "Cannot load Texture : ../res/units_red.png" << endl;
        if (!unitsBlue.loadFromFile("../res/units_blue.png"))
            cout << "Cannot load Texture : ../res/units_blue.png" << endl;
        if (!lifeBar.loadFromFile("../res/units_red.png"))
            cout << "Cannot load Texture : ../res/units_red.png" << endl;
        vector<sf::Sprite> sprites;
        for(Character character :  player.getUnits()){
            sf::Sprite unit;
            sf::Sprite life;
            life.setTexture(lifeBar);
            if(player.getId()== 1) {
                unit.setTexture(unitsRed);
            } else {
                unit.setTexture(unitsBlue);
            }
            life.setTextureRect(sf::IntRect(0, 72, 16.0*(static_cast<float >(character.getHp())/ static_cast<float >(character.getWeapon().getLpMax())), 2));
            unit.setTextureRect(sf::IntRect(0+(16*(character.getOrientation())), 0+(16*(character.getWeapon().getLevel())), 16, 16));
            life.setPosition(sf::Vector2f(character.getX()*320/20, character.getY()*320/20-4)); // position absolue
            unit.setPosition(sf::Vector2f(character.getX()*320/20, character.getY()*320/20)); // position absolue
            sprites.push_back(unit);
            sprites.push_back(life);
        }
        for(Tower towerFromPlayer :  player.getTowers()){
            sf::Sprite tower;
            sf::Sprite life;
            life.setTexture(lifeBar);
            if(player.getId()== 1) {
                tower.setTexture(unitsRed);
            } else {
                tower.setTexture(unitsBlue);
            }
            life.setTextureRect(sf::IntRect(0, 72, 16.0*(static_cast<float >(towerFromPlayer.getHp())/100.0), 2));
            tower.setTextureRect(sf::IntRect(0, 64, 16, 16));
            life.setPosition(sf::Vector2f(towerFromPlayer.getX() * 320 / 20, towerFromPlayer.getY() * 320 / 20-4)); // position absolue
            tower.setPosition(sf::Vector2f(towerFromPlayer.getX() * 320 / 20, towerFromPlayer.getY() * 320 / 20)); // position absolue
            
            sprites.push_back(tower);
            sprites.push_back(life);
        }
        for(ApparitionArea apparitionAreaFromPlayer :  player.getApparitionAreas()){
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
void Scene::updateTrajectory(vector<engine::Node> nodes) {
    if(isWindowAvailable(window)){
        LayerRender trajectory;
        vector<int> data(400, 0);
        for(engine::Node node : nodes){
            data[node.x + node.y*20] = 1;
        }
        int arr[400];
        copy(data.begin(), data.end(), arr);
        if (!trajectory.load("../res/trajectory.png", sf::Vector2u(16, 16), arr, 20, 20))
            cout << "Cannot load map" << endl;
        this->trajectory = trajectory;
        updateAll();
    }
}
void Scene::updateAttackField(vector<int> field) {
    if(isWindowAvailable(window)){
        LayerRender attackField;
        int arr[field.size()];
        copy(field.begin(), field.end(), arr);
        if (!attackField.load("../res/attack_field.png", sf::Vector2u(16, 16), arr, 20, 20))
            cout << "Cannot load map" << endl;
        this->attackField = attackField;
        updateAll();
    }
}
void Scene::stateChanged (const StateEvent &e, GameState &gameState){
    switch(e.getStateEventID()){
        case StateEventID::WORLD:
            //cout << "Notification from gameState for WORLD" << endl;
            this->updateWorld(gameState.getWorld());
            break;
            case StateEventID::PLAYER1://cout << "Notification from gameState for PLAYER1" << endl;
            this->updatePlayers(gameState.getPlayer1());
            break;
            case StateEventID::PLAYER2://cout << "Notification from gameState for PLAYER2" << endl;
            this->updatePlayers(gameState.getPlayer2());
            break;
        case StateEventID::SELECTED_UNIT:
            //cout << "Notification from gameState for SELECTED_UNIT" << endl;
        if(gameState.getSelectedUnit() != nullptr){
           if(!gameState.getAttackMode()){
               this->updateTrajectory(vector<Node>{Node{.x = gameState.getSelectedUnit().get()->getX(), .y = gameState.getSelectedUnit().get()->getY()}});
           } else {
               this->updateTrajectory(vector<Node>{});
           }
        } else {
            this->updateTrajectory(vector<Node>{});
        }
            break;
        case StateEventID::ATTACK_MODE://cout << "Notification from gameState for PLAYER2" << endl;
            if(gameState.getSelectedUnit() != nullptr){
                if(gameState.getAttackMode()){
                    this->updateAttackField(DisplayAttack::createField(gameState.getSelectedUnit().get(), gameState.getWorld()));
                } else {
                    this->updateAttackField(vector<int>(400,0));
                }
            } else {
                this->updateAttackField(vector<int>(400,0));
            }
            break;
            case StateEventID::ACTIVEPLAYER:// TODO Apply rendering coming from new active player
            break;
            case StateEventID::TURN:// TODO Update IHM who shows the number of turn
            break;
            default:cout << "Cannot read notification from gameState : "<< e.getStateEventID() << endl;
    }
}
bool Scene::isWindowAvailable(sf::RenderWindow &window) {
    if (!window.isOpen()) {
        cout << "window reference in Scene is closed" << endl;
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
        window.draw(this->attackField);
        window.draw(this->trajectory);
        for(auto sprite : this->player1Render)
            window.draw(sprite);
        for(auto sprite : this->player2Render)
            window.draw(sprite);
        window.display();
    }
}

