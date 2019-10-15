#include "Tower.h"

namespace state {

    Tower::Tower() : GameObject(), lp(100) {

    }

    Tower::Tower(int lp, int x, int y) :GameObject(x, y)  {
        this->lp = lp;
    }

    int Tower::getLp() {
        return this->lp;
    }

    void Tower::setLp(int lifePoints) {
        this->lp = lifePoints;
    }
}

