#include "Character.h"

namespace state {


    Character::Character() : GameObject(), lp{100} {

    }

    Character::Character(int x, int y, int lp) :GameObject(x, y) {
        this->lp = lp;
    }

    int Character::getLp() const {
        return this->lp;
    }

    void Character::setLp(int lifePoints) {
        this->lp = lifePoints;
    }

    const Weapon &Character::getWeapon() const {
        return this->weapon;
    }

    void Character::setWeapon(const Weapon &weapon) {
        this->weapon = weapon;
    }
}
