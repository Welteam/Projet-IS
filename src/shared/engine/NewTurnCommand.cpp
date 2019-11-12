#include <state.h>
#include <vector>
#include <iostream>
#include "NewTurnCommand.h"

using namespace engine;
using namespace state;
using namespace std;

NewTurnCommand::NewTurnCommand() {
}

void NewTurnCommand::execute(state::GameState &gameState) {
    state::Player player = gameState.getActivePlayer();
    vector<state::Character> newUnits;
    for(auto unit : player.getUnits()){
        unit.hasAttacked = false;
        unit.resetPm();
        newUnits.push_back(unit);
    }
    player.setUnits(newUnits);
    if(gameState.getActivePlayer().getId() == 1){
        gameState.setPlayer1(player);
        gameState.setActivePlayer(gameState.getPlayer2());
    } else if(gameState.getActivePlayer().getId() == 2) {
        gameState.setPlayer2(player);
        gameState.setActivePlayer(gameState.getPlayer1());
    } else {
        cout <<"Wrong player id in main .cpp key T pressed = " << gameState.getActivePlayer().getId() << endl;
    }
}
