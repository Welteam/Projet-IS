
#include "Weapon.h"

namespace state {

    Weapon::Weapon() : direction(0), pm(0), damage(0), rangeMin(0), rangeMax(0), damageAreaMax(0) {
        direction = new DirectionType();
    }

    Weapon::Weapon(int pm, int damage, int rangeMin, int rangeMax, int damageAreaMax, DirectionType direction) {
        this->pm = pm;
        this->damage = damage;
        this->rangeMin = rangeMin;
        this->rangeMax = rangeMax;
        this->damageAreaMax = damageAreaMax;
        *this->direction = direction;
    }

    int Weapon::getPm() {
        return this->pm;
    }

    void Weapon::setPm(int pm) {
        this->pm = pm;
    }

    int Weapon::getDamage() {
        return this->damage;
    }

    void Weapon::setDamage(int damage) {
        this->damage = damage;
    }

    int Weapon::getRangeMin() {
        return this->rangeMin;
    }

    int Weapon::getRangeMax() {
        return this->rangeMax;
    }

    void Weapon::setRangeMax(int rangeMax) {
        this->rangeMax = rangeMax;
    }

    int Weapon::getDamageAreaMax() {
        return this->damageAreaMax;
    }

    void Weapon::setDamageAreaMax(int damageAreaMax) {
        this->damageAreaMax = damageAreaMax;
    }

    DirectionType Weapon::getDirection() const {
        return *this->direction;
    }

    void Weapon::setDirection(DirectionType direction) {
        *this->direction = direction;
    }

}

