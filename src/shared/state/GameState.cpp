
#include "GameState.h"
#include <utility>

namespace state{

    GameState::GameState() : world{World()}, turn{0}, player1{Player()}, player2{Player()}{

    }

    GameState::GameState(std::string fileName) : world{World(std::move(fileName))}, turn{0}, player1{Player()}, player2{Player()}{

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
}
