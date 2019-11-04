#include "Engine.h"

namespace engine {

    Engine::Engine(state::GameState gameState) : gameState{std::move(gameState)} {

    }

    state::GameState &Engine::getGameState() {
        return gameState;
    }
}

