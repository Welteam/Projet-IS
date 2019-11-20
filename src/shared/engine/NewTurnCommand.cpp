#include <state.h>
#include <vector>
#include <iostream>
#include "NewTurnCommand.h"
#include "Cordinate.cpp"

using namespace engine;
using namespace state;
using namespace std;

NewTurnCommand::NewTurnCommand() = default;

void NewTurnCommand::execute(GameState &gameState) {
    Player activePlayer = gameState.getActivePlayer();
    vector<Character> newUnits;
    for(auto unit : activePlayer.getUnits()){
        unit.setHasAttacked(false);
        unit.resetPm();
        newUnits.push_back(unit);
    }
    activePlayer.setUnits(newUnits);
    if(gameState.getActivePlayer().getId() == 1){
        gameState.setPlayer1(activePlayer);

        Player nextPlayer = gameState.getPlayer2();
        vector<Character> newSpawnUnits =  gameState.getPlayer2().getUnits();
        vector<pair<int, int>> fourWay;
        bool unitSpawned = false;
        fourWay.emplace_back(-1, 0);
        fourWay.emplace_back(0, -1);
        fourWay.emplace_back(0, 1);
        fourWay.emplace_back(1, 0);
        for(auto spawnArea : nextPlayer.getApparitionAreas()){
            if(!unitSpawned) {
                for (auto coord : fourWay) {
                    if (!unitSpawned) {
                        int newX = coord.first;
                        int newY = coord.second;
                        if (spawnArea.getY() + newY < 0 || spawnArea.getY() + newY > (Y_MAX / Y_STEP) - 1) {
                            newY = 0;
                        }
                        if (spawnArea.getX() + newX < 0 || spawnArea.getX() + newX > (X_MAX / X_STEP) - 1) {
                            newX = 0;
                        }
                        if (Cordinate::isValid(spawnArea.getX() + newX, spawnArea.getY() + newY, gameState.getWorld(),
                                               gameState.getGameObjects())) {
                            newSpawnUnits.emplace_back(spawnArea.getX() + newX, spawnArea.getY() + newY);
                            unitSpawned = true;
                        }
                    }
                }
            }
        }
        nextPlayer.setUnits(newSpawnUnits);
        gameState.setPlayer2(nextPlayer);

        gameState.setActivePlayer(gameState.getPlayer2());
        gameState.setTurn(gameState.getTurn()+1);
    } else if(gameState.getActivePlayer().getId() == 2) {
        gameState.setPlayer2(activePlayer);


        Player nextPlayer = gameState.getPlayer1();
        vector<Character> newSpawnUnits =  gameState.getPlayer1().getUnits();
        vector<pair<int, int>> fourWay;
        bool unitSpawned = false;
        fourWay.emplace_back(-1, 0);
        fourWay.emplace_back(0, -1);
        fourWay.emplace_back(0, 1);
        fourWay.emplace_back(1, 0);
        for(auto spawnArea : nextPlayer.getApparitionAreas()){
            if(!unitSpawned) {
                for (auto coord : fourWay) {
                    if (!unitSpawned) {
                        int newX = coord.first;
                        int newY = coord.second;
                        if (spawnArea.getY() + newY < 0 || spawnArea.getY() + newY > (Y_MAX / Y_STEP) - 1) {
                            newY = 0;
                        }
                        if (spawnArea.getX() + newX < 0 || spawnArea.getX() + newX > (X_MAX / X_STEP) - 1) {
                            newX = 0;
                        }
                        if (Cordinate::isValid(spawnArea.getX() + newX, spawnArea.getY() + newY, gameState.getWorld(),
                                               gameState.getGameObjects())) {
                            newSpawnUnits.emplace_back(spawnArea.getX() + newX, spawnArea.getY() + newY);
                            unitSpawned = true;
                        }
                    }
                }
            }
        }
        nextPlayer.setUnits(newSpawnUnits);
        gameState.setPlayer1(nextPlayer);



        gameState.setActivePlayer(gameState.getPlayer1());
        gameState.setTurn(gameState.getTurn()+1);
    } else {
        cout <<"Wrong activePlayer id in main .cpp key T pressed = " << gameState.getActivePlayer().getId() << endl;
    }
}
