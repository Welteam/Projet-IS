#include "Engine.h"
#include "../state/GameState.h"

namespace engine {

    Engine::Engine(state::GameState gameState) : gameState{std::move(gameState)} {

    }

    state::GameState &Engine::getGameState() {
        return this->gameState;
    }

    std::shared_ptr<state::Character> Engine::getSelectedUnit() const {
        return selectedUnit;
    }

    void Engine::setSelectedUnit(std::shared_ptr<state::Character> selectedUnit) {
        attackMode = false;
        this->selectedUnit = selectedUnit;
    }

    void Engine::unselectedUnit() {
        selectedUnit.reset();
        attackMode = false;
    }

    bool Engine::getAttackMode() const {
        return attackMode;
    }

    void Engine::setAttackMode(bool attackMode) {
        this->attackMode = attackMode;
    }


}

