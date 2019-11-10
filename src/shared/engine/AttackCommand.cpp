#include "AttackCommand.h"
#include <iostream>
#include <state.h>
#include "engine.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"

using namespace std;
using namespace state;
using namespace engine;

engine::AttackCommand::AttackCommand(std::shared_ptr<state::Character> selectedUnit, int targetX, int targetY) {
    this->selectedUnit = selectedUnit;
    this->target = Node{.x = targetX, .y = targetY};
}

void engine::AttackCommand::execute(state::GameState &gameState) {
    cout << "unité déjà en mode attaque : attaque !!!!" << endl;
    vector<int> attackField = DisplayAttack::createField(selectedUnit.get(), gameState.getWorld());
    if(attackField[target.x+target.y*gameState.getWorld().getYMax()] == 1){
        vector<Node> damageArea = DisplayAttack::createDamageArea(target.x, target.y, selectedUnit.get(), gameState.getWorld());
        if(gameState.getActivePlayer().getId() == 2){
            for(auto node : damageArea){
                Player player1 = gameState.getPlayer1();
                vector<Character> newUnits1;
                vector<Tower> newTowers1;
                int damageX = node.x;
                int damageY = node.y;
                cout << "node x = " << damageX << " et y = " << damageY << endl;
                for(auto unit : gameState.getPlayer1().getUnits()){
                    if((unit.getX() == damageX && unit.getY() == damageY)){
                        unit.setHp(unit.getHp()- selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(unit.getHp() > 0){
                        newUnits1.push_back(unit);
                    } else {
                        Player playerAttacking = gameState.getPlayer2();
                        shared_ptr<Character> unitEvolve = selectedUnit;
                        vector<Character> newAttackingUnits;
                        for(auto unitAttacking : gameState.getPlayer2().getUnits()) {
                            if ((unitAttacking.getX() == unitEvolve.get()->getX() && unitAttacking.getY() == unitEvolve.get()->getY())) {
                                unitAttacking.levelUp();
                                if(unit.getX() < unitAttacking.getX()){
                                    unitAttacking.setOrientation(Orientation::WEST);
                                } else {
                                    unitAttacking.setOrientation(Orientation::EST);
                                }
                            }
                            newAttackingUnits.push_back(unitAttacking);
                        }
                        playerAttacking.setUnits(newAttackingUnits);
                        gameState.setPlayer2(playerAttacking);
                        gameState.setActivePlayer(gameState.getPlayer2());
                    }
                }
                for(auto tower : gameState.getPlayer1().getTowers()){
                    if((tower.getX() == damageX && tower.getY() == damageY)){
                        tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(tower.getHp() > 0){
                        newTowers1.push_back(tower);
                    } else {
                        Player playerAttacking = gameState.getPlayer2();
                        shared_ptr<Character> unitEvolve = selectedUnit;
                        vector<Character> newAttackingUnits;
                        for(auto unitAttacking : gameState.getPlayer2().getUnits()) {
                            if ((unitAttacking.getX() == unitEvolve.get()->getX() && unitAttacking.getY() == unitEvolve.get()->getY())) {
                                unitAttacking.levelUp();
                                if(tower.getX() < unitAttacking.getX()){
                                    unitAttacking.setOrientation(Orientation::WEST);
                                } else {
                                    unitAttacking.setOrientation(Orientation::EST);
                                }
                            }
                            newAttackingUnits.push_back(unitAttacking);
                        }
                        playerAttacking.setUnits(newAttackingUnits);
                        gameState.setPlayer2(playerAttacking);
                        gameState.setActivePlayer(gameState.getPlayer2());
                    }
                }
                player1.setUnits(newUnits1);
                player1.setTowers(newTowers1);
                gameState.setPlayer1(player1);

                Player player2 = gameState.getPlayer2();
                vector<Character> newUnits2;
                vector<Tower> newTowers2;
                for(auto unit : gameState.getPlayer2().getUnits()){
                    if((unit.getX() == damageX && unit.getY() == damageY)){
                        unit.setHp(unit.getHp()- selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(unit.getHp() > 0){
                        newUnits2.push_back(unit);
                    }
                }
                for(auto tower : gameState.getPlayer2().getTowers()){
                    if((tower.getX() == damageX && tower.getY() == damageY)){
                        tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(tower.getHp() > 0){
                        newTowers2.push_back(tower);
                    }
                }
                player2.setUnits(newUnits2);
                player2.setTowers(newTowers2);
                gameState.setPlayer2(player2);
                gameState.setActivePlayer(gameState.getPlayer2());
            }
        } else {
            for(auto node : damageArea){
                Player player2 = gameState.getPlayer2();
                vector<Character> newUnits2;
                vector<Tower> newTowers2;
                int damageX = node.x;
                int damageY = node.y;
                cout << "node x = " << damageX << " et y = " << damageY << endl;
                for(auto unit : gameState.getPlayer2().getUnits()){
                    if((unit.getX() == damageX && unit.getY() == damageY)){
                        cout << "unité touchée en x = " << unit.getX() << " et y = " << unit.getY() << endl;
                        unit.setHp(unit.getHp()- selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(unit.getHp() > 0){
                        newUnits2.push_back(unit);
                    } else {
                        Player playerAttacking = gameState.getPlayer1();
                        shared_ptr<Character> unitEvolve = selectedUnit;
                        vector<Character> newAttackingUnits;
                        for(auto unitAttacking : gameState.getPlayer1().getUnits()) {
                            if ((unitAttacking.getX() == unitEvolve.get()->getX() && unitAttacking.getY() == unitEvolve.get()->getY())) {
                                unitAttacking.levelUp();
                                if(unit.getX() < unitAttacking.getX()){
                                    unitAttacking.setOrientation(Orientation::WEST);
                                } else {
                                    unitAttacking.setOrientation(Orientation::EST);
                                }
                            }
                            newAttackingUnits.push_back(unitAttacking);
                        }
                        playerAttacking.setUnits(newAttackingUnits);
                        gameState.setPlayer1(playerAttacking);
                        gameState.setActivePlayer(gameState.getPlayer1());
                    }
                }
                for(auto tower : gameState.getPlayer2().getTowers()){
                    if((tower.getX() == damageX && tower.getY() == damageY)){
                        tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(tower.getHp() > 0){
                        newTowers2.push_back(tower);
                    } else {
                        Player playerAttacking = gameState.getPlayer1();
                        shared_ptr<Character> unitEvolve = selectedUnit;
                        vector<Character> newAttackingUnits;
                        for(auto unitAttacking : gameState.getPlayer1().getUnits()) {
                            if ((unitAttacking.getX() == unitEvolve.get()->getX() && unitAttacking.getY() == unitEvolve.get()->getY())) {
                                unitAttacking.levelUp();
                                if(tower.getX() < unitAttacking.getX()){
                                    unitAttacking.setOrientation(Orientation::WEST);
                                } else {
                                    unitAttacking.setOrientation(Orientation::EST);
                                }
                            }
                            newAttackingUnits.push_back(unitAttacking);
                        }
                        playerAttacking.setUnits(newAttackingUnits);
                        gameState.setPlayer1(playerAttacking);
                        gameState.setActivePlayer(gameState.getPlayer1());
                    }
                }
                player2.setUnits(newUnits2);
                player2.setTowers(newTowers2);
                gameState.setPlayer2(player2);

                Player player1 = gameState.getPlayer1();
                vector<Character> newUnits1;
                vector<Tower> newTowers1;
                for(auto unit : gameState.getPlayer1().getUnits()){
                    if((unit.getX() == damageX && unit.getY() == damageY)){
                        unit.setHp(unit.getHp()- selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(unit.getHp() > 0){
                        newUnits1.push_back(unit);
                    }
                }
                for(auto tower : gameState.getPlayer1().getTowers()){
                    if((tower.getX() == damageX && tower.getY() == damageY)){
                        tower.setHp(tower.getHp() - selectedUnit.get()->getWeapon().getDamage());
                    }
                    if(tower.getHp() > 0){
                        newTowers1.push_back(tower);
                    }
                }
                player1.setUnits(newUnits1);
                player1.setTowers(newTowers1);
                gameState.setPlayer1(player1);
                gameState.setActivePlayer(gameState.getPlayer1());
            }
        }
    }
}
