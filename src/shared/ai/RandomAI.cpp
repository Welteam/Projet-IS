#include <memory>
#include "RandomAI.h"
#include <iostream>
#include <engine.h>
#include <engine/AttackCommand.h>
#include <engine/MoveCommand.h>
#include <cstdlib>
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"

using namespace state;
using namespace std;
using namespace engine;

void ai::RandomAI::run(Engine &e) {
    for(auto unit : e.getGameState().getActivePlayer().getUnits()){
        bool foundUnitMove = false;
        e.setSelectedUnit(make_shared<Character>(unit));
        while(!foundUnitMove){
            if(e.getSelectedUnit() != nullptr){
                int maxX = e.getSelectedUnit().get()->getX()+e.getSelectedUnit().get()->getPm();
                int minX = e.getSelectedUnit().get()->getX()-e.getSelectedUnit().get()->getPm();
                int maxY = e.getSelectedUnit().get()->getY()+e.getSelectedUnit().get()->getPm();
                int minY = e.getSelectedUnit().get()->getY()-e.getSelectedUnit().get()->getPm();
                int randomMoveX = rand()%(maxX-minX + 1) + minX;
                int randomMoveY = rand()%(maxY-minY + 1) + minY;
                cout << "random move x" << randomMoveX << endl;
                cout << "random move y" << randomMoveY << endl;
                Node depart = {.x =  e.getSelectedUnit().get()->getX(), .y = e.getSelectedUnit().get()->getY()};
                Node destination = {.x = randomMoveX, .y = randomMoveY};
                vector<Node> trajectory = Cordinate::aStar(depart, destination, e.getGameState().getWorld(), e.getGameState().getGameObjects(), e.getSelectedUnit().get()->getPm());
                if(trajectory.size() > 1){
                    shared_ptr<Command> move = make_shared<MoveCommand>(e.getSelectedUnit(), randomMoveX, randomMoveY);
                    e.addCommand(move, 1);
                    e.runCommands(true);
                    foundUnitMove = true;
                } else {
                    cout << "AI didn't found a move, it tries again" << endl;
                }

            } else {
                cout << "L'unité ia ne peut pas être sélectionée." << endl;
            }
        }
        while(!unit.hasAttacked){
            if(e.getSelectedUnit() != nullptr){
                int maxX = e.getSelectedUnit().get()->getX()+e.getSelectedUnit().get()->getWeapon().getRangeMax();
                int minX = e.getSelectedUnit().get()->getX()-e.getSelectedUnit().get()->getWeapon().getRangeMax();
                int maxY = e.getSelectedUnit().get()->getY()+e.getSelectedUnit().get()->getWeapon().getRangeMax();
                int minY = e.getSelectedUnit().get()->getY()-e.getSelectedUnit().get()->getWeapon().getRangeMax();
                int randomAttackX = rand()%(maxX-minX + 1) + minX;
                int randomAttackY = rand()%(maxY-minY + 1) + minY;
                cout << "random attack x" << randomAttackX << endl;
                cout << "random attack y" << randomAttackY << endl;
                vector<int> attackField = DisplayAttack::createField(e.getSelectedUnit().get(), e.getGameState().getWorld());
                if(attackField[randomAttackX+ randomAttackY * e.getGameState().getWorld().getYMax()] == 1){
                    shared_ptr<Command> attack = make_shared<AttackCommand>(e.getSelectedUnit(), randomAttackX, randomAttackY);
                    e.addCommand(attack, 1);
                    e.runCommands(true);
                    e.unselectedUnit();
                    unit.hasAttacked = true;
                } else {
                    cout << "AI didn't found a target, it tries again" << endl;
                }

            } else {
                cout << "L'unité ia ne peut pas être sélectionée." << endl;
            }
        }
    }
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands(true);
}

