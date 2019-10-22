#include "Shotgun.h"

namespace state {

    Shotgun::Shotgun() : Weapon(7, 60, 1, 5, 2){

    }

    int Shotgun::getDirection() const {
        return DirectionType::CROSS;
    }

}

