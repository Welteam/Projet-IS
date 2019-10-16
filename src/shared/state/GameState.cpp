
#include "GameState.h"

#include <utility>

namespace state{

    GameState::GameState() : turn{0}, world{World()}, player1{Player()}, player2{Player()}{

    }

    GameState::GameState(std::string fileName) : turn{0}, world{World(std::move(fileName))}, player1{Player()}, player2{Player()}{

    }

    /*void GameState::saveWorld() {

    }*/

    int GameState::getTurn() {
        return this->turn;
    }

    void GameState::setTurn(int turn) {
        this->turn = turn;
    }

    World GameState::getWorld() {
        return this->world;
    }

    void GameState::setWorld(World world) {
        this->world = std::move(world);
    }

    Player GameState::getPlayer1() {
        return this->player1;
    }

    void GameState::setPlayer1(Player player1) {
        this->player1 = std::move(player1);
    }

    Player GameState::getPlayer2() {
        return this->player2;
    }

    void GameState::setPlayer2(Player player2) {
        this->player2 = std::move(player2);
    }

    /*const Player &GameState::getActivePlayer() const {
        return *this->activePlayer;
    }

    void GameState::setActivePlayer(const Player &activePlayer) {
        *this->activePlayer = activePlayer;
    }*/
}
