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

    int GameState::getTurn() const {
        return this->turn;
    }

    void GameState::setTurn(int newTurn) {
        this->turn = newTurn;
        this->notifyObservers(StateEvent{TURN}, *this);
    }

    Player GameState::getPlayer1() const {
        return this->player1;
    }

    void GameState::setPlayer1(Player newPlayer1) {
        this->player1 = std::move(newPlayer1);
        this->notifyObservers(StateEvent{PLAYER1}, *this);
    }

    Player GameState::getPlayer2() const {
        return this->player2;
    }

    void GameState::setPlayer2(Player newPlayer2) {
        this->player2 = std::move(newPlayer2);
        this->notifyObservers(StateEvent{PLAYER2}, *this);
    }

    const Player &GameState::getActivePlayer() const {
        return this->activePlayer;
    }

    void GameState::setActivePlayer(const Player &newActivePlayer) {
        this->activePlayer = newActivePlayer;
        this->notifyObservers(StateEvent{ACTIVEPLAYER}, *this);
    }

    const World &GameState::getWorld() const {
        return this->world;
    }

    void GameState::setWorld(const World &newWorld) {
        this->world = newWorld;
        this->notifyObservers(StateEvent{WORLD}, *this);
    }

    std::vector<GameObject> GameState::getGameObjects() {
        std::vector<GameObject> gameObjects;
        for(const auto& gameObject : getPlayer1().getUnits())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer1().getTowers())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer1().getApparitionAreas())
            gameObjects.push_back(gameObject);
        for(const auto& gameObject : getPlayer2().getUnits())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer2().getTowers())
            gameObjects.push_back(gameObject);
        for(auto gameObject : getPlayer2().getApparitionAreas())
            gameObjects.push_back(gameObject);
        return gameObjects;
    }

    std::shared_ptr<state::Character> GameState::getSelectedUnit() {
        return selectedUnit;
    }

    void GameState::setSelectedUnit(std::shared_ptr<state::Character> newSelectedUnit) {
        attackMode = false;
        this->selectedUnit = std::move(newSelectedUnit);
        this->notifyObservers(StateEvent{SELECTED_UNIT}, *this);
        this->notifyObservers(StateEvent{ATTACK_MODE}, *this);
    }

    void GameState::unselectedUnit() {
        selectedUnit.reset();
        attackMode = false;
        this->notifyObservers(StateEvent{SELECTED_UNIT}, *this);
        this->notifyObservers(StateEvent{ATTACK_MODE}, *this);
    }

    bool GameState::getAttackMode() const {
        return attackMode;
    }

    void GameState::setAttackMode(bool newAttackMode) {
        this->attackMode = newAttackMode;
        this->notifyObservers(StateEvent{ATTACK_MODE}, *this);
    }
}
