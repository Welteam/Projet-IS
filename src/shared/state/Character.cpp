#include "Character.h"

namespace state {


    Character::Character() : GameObject(), lp{100}, characterState{CharacterState::IDLE} {

    }

    Character::Character(int x, int y, int lp, CharacterState state) :GameObject(x, y) {
        this->lp = lp;
        this->characterState = state;
    }

    int Character::getLp() {
        return this->lp;
    }

    void Character::setLp(int lifePoints) {
        this->lp = lifePoints;
    }

    CharacterState Character::getCharacterState() {
        return this->characterState;
    }

    void Character::setCharacterState(CharacterState characterState) {
        this->characterState = characterState;
    }

    /*const Weapon &Character::getWeapon() const {
        return <#initializer#>;
    }

    void Character::setWeapon(const Weapon &weapon) {

    }*/
}
