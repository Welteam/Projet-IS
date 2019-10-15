#include "Player.h"

namespace state{


    Player::Player() : id(0) {

    }

    Player::Player(int id, std::vector<Character> units, std::vector<Tower> towers,
                   std::vector<ApparitionArea> apparitionAreas) {
        this->id = id;
        this->units = units;
        this->towers = towers;
        this->apparitionAreas = apparitionAreas;

    }

    int Player::getId() {
        return this->id;
    }

    std::vector<Character> Player::getUnits() {
        return this->units;
    }

    std::vector<Tower> Player::getTowers() {
        return this->towers;
    }

    std::vector<ApparitionArea> Player::getApparitionAreas() {
        return this->apparitionAreas;
    }

    void Player::setUnits(std::vector<Character> characters) {
        this->units = characters;
    }

    void Player::setTowers(std::vector<Tower> towers) {
        this->towers = towers;
    }

    void Player::setApparitionAreas(std::vector<ApparitionArea> apparitionAreas) {
        this->apparitionAreas = apparitionAreas;
    }
}
