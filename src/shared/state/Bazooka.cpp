#include "Bazooka.h"


namespace state {

    Bazooka::Bazooka() : Weapon(5, 10, 1, 5, 0){
    }

    int Bazooka::getDirection() {
        return DirectionType::FULL;
    }

}
