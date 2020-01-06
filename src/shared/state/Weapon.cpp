
#include "Weapon.h"

namespace state {

    Weapon::Weapon() {
        // Value is too long so dia2code cannot declare it as a default value
        this->direction = {DirectionType::FULL, DirectionType::CROSS, DirectionType::FULL, DirectionType::OCTOPUS};
    }

    int Weapon::getPmMax() const {
        return this->pmMax[this->level];
    }

    int Weapon::getDamage() const {
        return this->damage[this->level];
    }

    int Weapon::getRangeMin() const {
        return this->rangeMin[this->level];
    }

    int Weapon::getRangeMax() const {
        return this->rangeMax[this->level];
    }

    int Weapon::getDamageAreaMax() const {
        return this->damageAreaMax[this->level];
    }

    DirectionType Weapon::getDirection() const {
        return this->direction[this->level];
    }

    int Weapon::getLpMax() const {
        return this->lpMax[this->level];
    }

    int Weapon::getLevel() const {
        return this->level;
    }

    // TODO: add error
    void Weapon::levelUp() {
        if (this->level < 3) {
            this->level += 1;
        }
    }

    void Weapon::setLevel(int newLevel) {
        if (newLevel <= 3) {
            this->level = newLevel;
        }
    }



}

