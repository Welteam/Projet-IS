#include "Player.h"

#include <utility>

using namespace state;
using namespace std;


Player::Player(){
    this->id = 0;
}

Player::Player(int id, vector<Character> units, vector<Tower> towers,
        vector<ApparitionArea> apparitionAreas) {
    this->id = id;
    this->units = move(units);
    this->towers = move(towers);
    this->apparitionAreas = move(apparitionAreas);

}

int Player::getId() const {
    return this->id;
}

vector<Character> Player::getUnits() const {
    return this->units;
}

vector<Tower> Player::getTowers() const {
    return this->towers;
}

vector<ApparitionArea> Player::getApparitionAreas() const {
    return this->apparitionAreas;
}

void Player::setUnits(vector<Character> characters) {
    this->units = std::move(characters);
}

void Player::setTowers(vector<Tower> newTowers) {
    this->towers = std::move(newTowers);
}

void Player::setApparitionAreas(vector<ApparitionArea> newApparitionAreas) {
    this->apparitionAreas = std::move(newApparitionAreas);
}
