
#include "Weapon.h"

namespace state {

    Weapon::Weapon() : level{0}, pm{0}, damage{0}, rangeMin{0}, rangeMax{0}, damageAreaMax{0}, direction{DirectionType::FULL} {

    }

    Weapon::Weapon(int level,int pm, int damage, int rangeMin, int rangeMax, int damageAreaMax, DirectionType direction) {
        this->level = level;
        this->pm = pm;
        this->damage = damage;
        this->rangeMin = rangeMin;
        this->rangeMax = rangeMax;
        this->damageAreaMax = damageAreaMax;
        this->direction = direction;
    }

    int Weapon::getPm() const {
        return this->pm;
    }

    void Weapon::setPm(int pm) {
        this->pm = pm;
    }

    int Weapon::getDamage() const {
        return this->damage;
    }

    void Weapon::setDamage(int damage) {
        this->damage = damage;
    }

    int Weapon::getRangeMin() const {
        return this->rangeMin;
    }

    int Weapon::getRangeMax() const {
        return this->rangeMax;
    }

    void Weapon::setRangeMax(int rangeMax) {
        this->rangeMax = rangeMax;
    }

    int Weapon::getDamageAreaMax() const {
        return this->damageAreaMax;
    }

    void Weapon::setDamageAreaMax(int damageAreaMax) {
        this->damageAreaMax = damageAreaMax;
    }

    int Weapon::getDirection() const {
        return this->direction;
    }

    int Weapon::getLevel() const {
        return this->level;
    }

    void Weapon::levelUp() {
        int currentLevel = this->getLevel();
        switch(currentLevel){
            case 0:
                this->level = 1; // Shotgun
                this->pm = 5;
                this->damage = 60;
                this->rangeMin = 0;
                this->rangeMax = 3;
                this->damageAreaMax = 0;
                this->direction = DirectionType::CROSS;
                break;
            case 1:
                this->level = 2; // Sniper
                this->pm = 4;
                this->damage = 40;
                this->rangeMin = 0;
                this->rangeMax = 8;
                this->damageAreaMax = 0;
                this->direction = DirectionType::FULL;
                break;
            case 2:
                this->level = 3; // Bazooka
                this->pm = 4;
                this->damage = 50;
                this->rangeMin = 3;
                this->rangeMax = 7;
                this->damageAreaMax = 2;
                this->direction = DirectionType::OCTOPUS;
                break;
        }

    }


}

