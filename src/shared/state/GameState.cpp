#include "GameState.h"
#include <utility>
#include <iostream>

namespace state{

    GameState::GameState() {
        this->world = World();
        this->turn = 0;
        this->player1 = Player();
        this->player2 = Player();
    }

    GameState::GameState(std::string fileName) {
        this->world = World(std::move(fileName));
        this->turn = 0;
        this->player1 = Player();
        this->player2 = Player();
    }

    void GameState::saveWorld() {

    }

    int GameState::getTurn() const {
        return this->turn;
    }

    void GameState::setTurn(int turn) {
        this->turn = turn;
        this->notifyObservers(StateEvent{TURN}, *this);
    }

    Player GameState::getPlayer1() const {
        return this->player1;
    }

    void GameState::setPlayer1(Player player1) {
        this->player1 = std::move(player1);
        this->notifyObservers(StateEvent{PLAYER1}, *this);
    }

    Player GameState::getPlayer2() const {
        return this->player2;
    }

    void GameState::setPlayer2(Player player2) {
        this->player2 = std::move(player2);
        this->notifyObservers(StateEvent{PLAYER2}, *this);
    }

    const Player &GameState::getActivePlayer() const {
        return this->activePlayer;
    }

    void GameState::setActivePlayer(const Player &activePlayer) {
        this->activePlayer = activePlayer;
        this->notifyObservers(StateEvent{ACTIVEPLAYER}, *this);
    }

    const World &GameState::getWorld() const {
        return this->world;
    }

    void GameState::setWorld(const World &world) {
        this->world = world;
        this->notifyObservers(StateEvent{WORLD}, *this);
    }

    std::vector<GameObject> GameState::getGameObjects() {
        std::vector<GameObject> gameObjects;
        for(auto gameObject : getPlayer1().getUnits())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer1().getTowers())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer1().getApparitionAreas())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer2().getUnits())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer2().getTowers())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer2().getApparitionAreas()){
            std::cout << " spawnArea en x = " << gameObject.getX() << " et y = " << gameObject.getY() << std::endl;
            gameObjects.push_back(gameObject);
        }
        return gameObjects;
    }
}
