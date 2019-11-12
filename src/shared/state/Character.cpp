#include "Character.h"

namespace state {


    Character::Character() : GameObject() {
        this->weapon = Weapon();
        this->pm = this->weapon.getPmMax();
    }

    Character::Character(int x, int y) : GameObject(x, y) {

    }

    // TODO: set weapon's level
    Character::Character(int x, int y, int hp, int pm) : GameObject(x, y) {
        this->weapon = Weapon{};
        this->hp = hp;
        this->pm = pm;
    }

    int Character::getHp() const {
        return this->hp;
    }

    void Character::setHp(int hp) {
        this->hp = hp;
    }

    const Weapon &Character::getWeapon() const {
        return this->weapon;
    }

    void Character::setWeapon(const Weapon &weapon) {
        this->weapon = weapon;
    }

    void Character::levelUp() {
        this->weapon.levelUp();
        this->hp += 20;
        if (this->hp > weapon.getLpMax())
            this->hp = weapon.getLpMax();
    }

    void Character::resetPm() {
        this->pm = this->weapon.getPmMax();
    }

    void Character::usePm(int pm) {
        this->pm -= pm;
    }

    int Character::getPm() const {
        return this->pm;
    }

    Orientation Character::getOrientation() {
        return this->orientation;
    }

    void Character::setOrientation(Orientation orientation) {
        this->orientation = orientation;
    }


}
