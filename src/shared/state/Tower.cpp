#include "Tower.h"

namespace state {

    Tower::Tower() : GameObject() {

    }

    Tower::Tower(int x, int y) : GameObject(x, y) {

    }

    Tower::Tower(int x, int y, int hp) :GameObject(x, y) {
        this->hp = hp;
    }

    int Tower::getHp() const {
        return this->hp;
    }

    void Tower::setHp(int newHp) {
        this->hp = newHp;
    }

}

