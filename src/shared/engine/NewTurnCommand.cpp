#include <state.h>
#include <vector>
#include <iostream>
#include "NewTurnCommand.h"
#include "Cordinate.cpp"

using namespace engine;
using namespace state;
using namespace std;

NewTurnCommand::NewTurnCommand() {
}

void NewTurnCommand::execute(GameState &gameState) {
    Player activePlayer = gameState.getActivePlayer();
    vector<Character> newUnits;
    for(auto unit : activePlayer.getUnits()){
        unit.hasAttacked = false;
        unit.resetPm();
        newUnits.push_back(unit);
    }
    activePlayer.setUnits(newUnits);
    if(gameState.getActivePlayer().getId() == 1){
        gameState.setPlayer1(activePlayer);
        /*Player nextPlayer = gameState.getPlayer2();
        vector<Character> newSpawnUnits =  gameState.getPlayer2().getUnits();

        for(auto spawnArea : nextPlayer.getApparitionAreas()){
            if(Cordinate::isValid()
        }
        nextPlayer.setUnits(newSpawnUnits);
        gameState.setPlayer2(nextPlayer);*/
        gameState.setActivePlayer(gameState.getPlayer2());
        gameState.setTurn(gameState.getTurn()+1);
    } else if(gameState.getActivePlayer().getId() == 2) {
        gameState.setPlayer2(activePlayer);
        gameState.setActivePlayer(gameState.getPlayer1());
        gameState.setTurn(gameState.getTurn()+1);
    } else {
        cout <<"Wrong activePlayer id in main .cpp key T pressed = " << gameState.getActivePlayer().getId() << endl;
    }
}
