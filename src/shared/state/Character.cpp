#include "Character.h"

namespace state {


    Character::Character() : GameObject(), weapon{Weapon()}, lp{100} {

    }

    // TODO: set weapon's level
    Character::Character(int x, int y, int lp) :GameObject(x, y), weapon{Weapon()} {
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

    void Character::levelUp() {
        this->weapon.levelUp();
        this->lp += 20;
        if (this->lp > weapon.getLpMax())
            this->lp = weapon.getLpMax();
    }
}
