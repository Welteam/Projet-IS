
#include "Weapon.h"

namespace state {

    // TODO: Remove setters and add setLevel method

    Weapon::Weapon() {

    }

    Weapon::Weapon(int level,int lpMax, int pm, int damage, int rangeMin, int rangeMax, int damageAreaMax, DirectionType direction) {
        this->level = level;
        this->lpMax = lpMax;
        this->pmMax = pm;
        this->damage = damage;
        this->rangeMin = rangeMin;
        this->rangeMax = rangeMax;
        this->damageAreaMax = damageAreaMax;
        this->direction = direction;
    }

    int Weapon::getPmMax() const {
        return this->pmMax;
    }

    int Weapon::getDamage() const {
        return this->damage;
    }

    int Weapon::getRangeMin() const {
        return this->rangeMin;
    }

    int Weapon::getRangeMax() const {
        return this->rangeMax;
    }

    int Weapon::getDamageAreaMax() const {
        return this->damageAreaMax;
    }

    DirectionType Weapon::getDirection() const {
        return this->direction;
    }

    int Weapon::getLevel() const {
        return this->level;
    }

    void Weapon::levelUp() {
        switch(this->level){
            case 0:
                this->level = 1; // Shotgun
                this->lpMax = 120;
                this->pmMax = 5;
                this->damage = 60;
                this->rangeMin = 0;
                this->rangeMax = 3;
                this->damageAreaMax = 2;
                this->direction = DirectionType::CROSS;
                break;
            case 1:
                this->level = 2; // Sniper
                this->lpMax = 140;
                this->pmMax = 4;
                this->damage = 40;
                this->rangeMin = 0;
                this->rangeMax = 8;
                this->damageAreaMax = 0;
                this->direction = DirectionType::FULL;
                break;
            case 2:
                this->level = 3; // Bazooka
                this->lpMax = 160;
                this->pmMax = 4;
                this->damage = 50;
                this->rangeMin = 3;
                this->rangeMax = 7;
                this->damageAreaMax = 3;
                this->direction = DirectionType::OCTOPUS;
                break;
        }

    }

    int Weapon::getLpMax() const {
        return this->lpMax;
    }

    void Weapon::setLevel(int level) {
        switch(level){
            case 1:
                this->level = 1; // Shotgun
                this->lpMax = 120;
                this->pmMax = 5;
                this->damage = 60;
                this->rangeMin = 0;
                this->rangeMax = 3;
                this->damageAreaMax = 0;
                this->direction = DirectionType::CROSS;
                break;
            case 2:
                this->level = 2; // Sniper
                this->lpMax = 140;
                this->pmMax = 4;
                this->damage = 40;
                this->rangeMin = 0;
                this->rangeMax = 8;
                this->damageAreaMax = 0;
                this->direction = DirectionType::DIAGONAL;
                break;
            case 3:
                this->level = 3; // Bazooka
                this->lpMax = 160;
                this->pmMax = 4;
                this->damage = 50;
                this->rangeMin = 3;
                this->rangeMax = 7;
                this->damageAreaMax = 2;
                this->direction = DirectionType::OCTOPUS;
                break;
        }
    }



}

