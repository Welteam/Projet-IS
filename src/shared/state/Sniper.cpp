#include "Sniper.h"

namespace state {

    Sniper::Sniper() : Weapon(2, 70, 1, 50, 0){

    }

    int Sniper::getDirection() const {
        return DirectionType::FULL;
    }

}

