#include <memory>
#include "RandomAI.h"
#include <iostream>
#include <engine/AttackCommand.h>
#include <engine/MoveCommand.h>
#include <cstdlib>
#include <engine/NewTurnCommand.h>
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"

using namespace state;
using namespace std;
using namespace engine;

void ai::RandomAI::run(Engine &e) {
    for(auto unit : e.getGameState().getActivePlayer().getUnits()){
        bool foundUnitMove = false;
        e.getGameState().setSelectedUnit(make_shared<Character>(unit));
        while(!foundUnitMove){
            if(e.getGameState().getSelectedUnit() != nullptr){
                int maxX = e.getGameState().getSelectedUnit().get()->getX()+e.getGameState().getSelectedUnit().get()->getPm();
                int minX = e.getGameState().getSelectedUnit().get()->getX()-e.getGameState().getSelectedUnit().get()->getPm();
                int maxY = e.getGameState().getSelectedUnit().get()->getY()+e.getGameState().getSelectedUnit().get()->getPm();
                int minY = e.getGameState().getSelectedUnit().get()->getY()-e.getGameState().getSelectedUnit().get()->getPm();
                int randomMoveX = rand()%(maxX-minX + 1) + minX;
                int randomMoveY = rand()%(maxY-minY + 1) + minY;
                cout << "random move x" << randomMoveX << endl;
                cout << "random move y" << randomMoveY << endl;
                Node depart = {.x =  e.getGameState().getSelectedUnit().get()->getX(), .y = e.getGameState().getSelectedUnit().get()->getY()};
                Node destination = {.x = randomMoveX, .y = randomMoveY};
                vector<Node> trajectory = Cordinate::aStar(depart, destination, e.getGameState().getWorld(), e.getGameState().getGameObjects(), e.getGameState().getSelectedUnit().get()->getPm());
                if(trajectory.size() > 1){
                    shared_ptr<Command> move = make_shared<MoveCommand>(e.getGameState().getSelectedUnit(), randomMoveX, randomMoveY);
                    e.addCommand(move, 1);
                    e.runCommands();
                    foundUnitMove = true;
                } else {
                    cout << "AI didn't found a move, it tries again" << endl;
                }

            }
        }
        while(!unit.hasAttacked){
            if(e.getGameState().getSelectedUnit() != nullptr){
                int maxX = e.getGameState().getSelectedUnit().get()->getX()+e.getGameState().getSelectedUnit().get()->getWeapon().getRangeMax();
                int minX = e.getGameState().getSelectedUnit().get()->getX()-e.getGameState().getSelectedUnit().get()->getWeapon().getRangeMax();
                int maxY = e.getGameState().getSelectedUnit().get()->getY()+e.getGameState().getSelectedUnit().get()->getWeapon().getRangeMax();
                int minY = e.getGameState().getSelectedUnit().get()->getY()-e.getGameState().getSelectedUnit().get()->getWeapon().getRangeMax();
                int randomAttackX = rand()%(maxX-minX + 1) + minX;
                int randomAttackY = rand()%(maxY-minY + 1) + minY;
                cout << "random attack x" << randomAttackX << endl;
                cout << "random attack y" << randomAttackY << endl;
                vector<int> attackField = DisplayAttack::createField(e.getGameState().getSelectedUnit().get(), e.getGameState().getWorld());
                if(attackField[randomAttackX+ randomAttackY * e.getGameState().getWorld().getYMax()] == 1){
                    shared_ptr<Command> attack = make_shared<AttackCommand>(e.getGameState().getSelectedUnit(), randomAttackX, randomAttackY);
                    e.addCommand(attack, 1);
                    e.runCommands();
                    e.getGameState().unselectedUnit();
                    unit.hasAttacked = true;
                } else {
                    cout << "AI didn't found a target, it tries again" << endl;
                }

            }
        }
    }
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands();
}

