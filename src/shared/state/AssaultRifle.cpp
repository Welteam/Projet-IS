#include "AssaultRifle.h"


namespace state {

    AssaultRifle::AssaultRifle() : Weapon(5, 10, 1, 5, 0){
    }

    int AssaultRifle::getDirection() const {
        return DirectionType::FULL;
    }

}


