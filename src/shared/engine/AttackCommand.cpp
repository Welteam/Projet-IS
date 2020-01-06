#include "AttackCommand.h"
#include <iostream>
#include <utility>
#include <state.h>
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"

using namespace std;
using namespace state;
using namespace engine;

AttackCommand::AttackCommand(std::shared_ptr<state::Character> selectedUnit, int targetX, int targetY) {
    this->selectedUnit = std::move(selectedUnit);
    this->target = Node{.x = targetX, .y = targetY};
}

bool AttackCommand::execute(state::GameState &gameState) {
    // cout << "unité déjà en mode attaque : attaque !!!!" << endl;
    if(!selectedUnit.get()->getHasAttacked()){
        vector<int> attackField = DisplayAttack::createField(selectedUnit.get(), gameState.getWorld(), gameState.getGameObjects());
        if(attackField[target.x+target.y*gameState.getWorld().getYMax()] == 1) {
            vector<Node> damageArea = DisplayAttack::createDamageArea(target.x, target.y, selectedUnit.get(),
                                                                      gameState.getWorld());
            if (gameState.getActivePlayer().getId() == 2) {
                for (auto node : damageArea) {
                    Player player1 = gameState.getPlayer1();
                    vector<Character> newUnits1;
                    vector<Tower> newTowers1;
                    int damageX = node.x;
                    int damageY = node.y;
                    for (auto unit : gameState.getPlayer1().getUnits()) {
                        if ((unit.getX() == damageX && unit.getY() == damageY)) {
                            unit.setHp(unit.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (unit.getHp() > 0) {
                            newUnits1.push_back(unit);
                        } else {
                            Player playerAttacking = gameState.getPlayer2();
                            shared_ptr<Character> unitEvolve = selectedUnit;
                            vector<Character> newAttackingUnits;
                            for (auto unitAttacking : gameState.getPlayer2().getUnits()) {
                                if ((unitAttacking.getX() == unitEvolve.get()->getX() &&
                                     unitAttacking.getY() == unitEvolve.get()->getY())) {
                                    unitAttacking.levelUp();
                                }
                                newAttackingUnits.push_back(unitAttacking);
                            }
                            playerAttacking.setUnits(newAttackingUnits);
                            gameState.setPlayer2(playerAttacking);
                            gameState.setActivePlayer(gameState.getPlayer2());
                        }
                    }
                    for (auto tower : gameState.getPlayer1().getTowers()) {
                        if ((tower.getX() == damageX && tower.getY() == damageY)) {
                            tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (tower.getHp() > 0) {
                            newTowers1.push_back(tower);
                        } else {
                            Player playerAttacking = gameState.getPlayer2();
                            shared_ptr<Character> unitEvolve = selectedUnit;
                            vector<Character> newAttackingUnits;
                            for (auto unitAttacking : gameState.getPlayer2().getUnits()) {
                                if ((unitAttacking.getX() == unitEvolve.get()->getX() &&
                                     unitAttacking.getY() == unitEvolve.get()->getY())) {
                                    unitAttacking.levelUp();
                                }
                                newAttackingUnits.push_back(unitAttacking);
                            }
                            playerAttacking.setUnits(newAttackingUnits);
                            gameState.setPlayer2(playerAttacking);
                            gameState.setActivePlayer(gameState.getPlayer2());
                        }
                    }
                    if(newUnits1.empty() || newTowers1.empty()){
                        cout << "JOUEUR 1 EN ROUGE A PERDUUUUUUUU !" << endl;
                        newUnits1.clear();
                        newTowers1.clear();
                    }
                    player1.setUnits(newUnits1);
                    player1.setTowers(newTowers1);
                    gameState.setPlayer1(player1);

                    Player player2 = gameState.getPlayer2();
                    vector<Character> newUnits2;
                    vector<Tower> newTowers2;
                    for (auto unit : gameState.getPlayer2().getUnits()) {
                        if ((unit.getX() == selectedUnit.get()->getX() && unit.getY() == selectedUnit.get()->getY())) {
                            if(abs(unit.getX()-target.x) > abs(unit.getY()-target.y)){
                                if((unit.getX()-target.x)  > 0){
                                    unit.setOrientation(state::Orientation::WEST);
                                } else if((unit.getX()-target.x)  < 0){
                                    unit.setOrientation(state::Orientation::EST);
                                }
                            } else if(abs(unit.getX()-target.x) < abs(unit.getY()-target.y)){
                                if((unit.getY()-target.y)  > 0){
                                    unit.setOrientation(state::Orientation::NORTH);
                                } else if((unit.getY()-target.y)  < 0){
                                    unit.setOrientation(state::Orientation::SOUTH);
                                }
                            } else {
                                if((unit.getX()-target.x)  > 0){
                                    unit.setOrientation(state::Orientation::WEST);
                                } else if((unit.getX()-target.x)  < 0){
                                    unit.setOrientation(state::Orientation::EST);
                                }
                            }
                            unit.setHasAttacked(true);
                        }
                        if ((unit.getX() == damageX && unit.getY() == damageY)) {
                            unit.setHp(unit.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (unit.getHp() > 0) {
                            newUnits2.push_back(unit);
                        }
                    }
                    for (auto tower : gameState.getPlayer2().getTowers()) {
                        if ((tower.getX() == damageX && tower.getY() == damageY)) {
                            tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (tower.getHp() > 0) {
                            newTowers2.push_back(tower);
                        }
                    }
                    player2.setUnits(newUnits2);
                    player2.setTowers(newTowers2);
                    gameState.setPlayer2(player2);
                    gameState.setActivePlayer(gameState.getPlayer2());
                }
            } else {
                for (auto node : damageArea) {
                    Player player2 = gameState.getPlayer2();
                    vector<Character> newUnits2;
                    vector<Tower> newTowers2;
                    int damageX = node.x;
                    int damageY = node.y;
                    for (auto unit : gameState.getPlayer2().getUnits()) {
                        if ((unit.getX() == damageX && unit.getY() == damageY)) {
                            unit.setHp(unit.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (unit.getHp() > 0) {
                            newUnits2.push_back(unit);
                        } else {
                            Player playerAttacking = gameState.getPlayer1();
                            shared_ptr<Character> unitEvolve = selectedUnit;
                            vector<Character> newAttackingUnits;
                            for (auto unitAttacking : gameState.getPlayer1().getUnits()) {
                                if ((unitAttacking.getX() == unitEvolve.get()->getX() &&
                                     unitAttacking.getY() == unitEvolve.get()->getY())) {
                                    unitAttacking.levelUp();
                                }
                                newAttackingUnits.push_back(unitAttacking);
                            }
                            playerAttacking.setUnits(newAttackingUnits);
                            gameState.setPlayer1(playerAttacking);
                            gameState.setActivePlayer(gameState.getPlayer1());
                        }
                    }
                    for (auto tower : gameState.getPlayer2().getTowers()) {
                        if ((tower.getX() == damageX && tower.getY() == damageY)) {
                            tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (tower.getHp() > 0) {
                            newTowers2.push_back(tower);
                        } else {
                            Player playerAttacking = gameState.getPlayer1();
                            shared_ptr<Character> unitEvolve = selectedUnit;
                            vector<Character> newAttackingUnits;
                            for (auto unitAttacking : gameState.getPlayer1().getUnits()) {
                                if ((unitAttacking.getX() == unitEvolve.get()->getX() &&
                                     unitAttacking.getY() == unitEvolve.get()->getY())) {
                                    unitAttacking.levelUp();
                                }
                                newAttackingUnits.push_back(unitAttacking);
                            }
                            playerAttacking.setUnits(newAttackingUnits);
                            gameState.setPlayer1(playerAttacking);
                            gameState.setActivePlayer(gameState.getPlayer1());
                        }
                    }
                    if(newUnits2.empty() || newTowers2.empty()){
                        cout << "JOUEUR 1 EN ROUGE A PERDUUUUUUUU !" << endl;
                        newUnits2.clear();
                        newTowers2.clear();
                    }
                    player2.setUnits(newUnits2);
                    player2.setTowers(newTowers2);
                    gameState.setPlayer2(player2);

                    Player player1 = gameState.getPlayer1();
                    vector<Character> newUnits1;
                    vector<Tower> newTowers1;
                    for (auto unit : gameState.getPlayer1().getUnits()) {
                        if (unit.getX() == selectedUnit.get()->getX() && unit.getY() == selectedUnit.get()->getY()) {
                            if(abs(unit.getX()-target.x) > abs(unit.getY()-target.y)){
                                if((unit.getX()-target.x)  > 0){
                                    unit.setOrientation(state::Orientation::WEST);
                                } else if((unit.getX()-target.x)  < 0){
                                    unit.setOrientation(state::Orientation::EST);
                                }
                            } else if(abs(unit.getX()-target.x) < abs(unit.getY()-target.y)){
                                if((unit.getY()-target.y)  > 0){
                                    unit.setOrientation(state::Orientation::NORTH);
                                } else if((unit.getY()-target.y)  < 0){
                                    unit.setOrientation(state::Orientation::SOUTH);
                                }
                            } else {
                                if((unit.getX()-target.x)  > 0){
                                    unit.setOrientation(state::Orientation::WEST);
                                } else if((unit.getX()-target.x)  < 0){
                                    unit.setOrientation(state::Orientation::EST);
                                }
                            }
                            unit.setHasAttacked(true);
                        }
                        if ((unit.getX() == damageX && unit.getY() == damageY)) {
                            unit.setHp(unit.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (unit.getHp() > 0) {
                            newUnits1.push_back(unit);
                        }
                    }
                    for (auto tower : gameState.getPlayer1().getTowers()) {
                        if ((tower.getX() == damageX && tower.getY() == damageY)) {
                            tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                        }
                        if (tower.getHp() > 0) {
                            newTowers1.push_back(tower);
                        }
                    }
                    player1.setUnits(newUnits1);
                    player1.setTowers(newTowers1);
                    gameState.setPlayer1(player1);
                    gameState.setActivePlayer(gameState.getPlayer1());
                }
            }
            gameState.launchDamageAnimation(damageArea);
        }
    }
}

void AttackCommand::serialize(Json::Value &root) {

}



