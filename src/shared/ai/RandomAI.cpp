#include <memory>
#include <random>
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
    double randomValue;
    std::random_device rd;
    std::mt19937 mt(rd());
    for(auto units : e.getGameState().getActivePlayer().getUnits()){
        while (true) {
            e.getGameState().setSelectedUnit(make_shared<Character>(units));
            if (e.getGameState().getSelectedUnit() == nullptr) {
                continue;
            }
            auto unit = e.getGameState().getSelectedUnit();
            std::uniform_real_distribution<double> dist(0.0, 5.0);
            randomValue = dist(mt);
            if ((randomValue < 2.0) & (unit.get()->getPm() > 0)) {
                while (true) {
                    int maxX = unit.get()->getX() + unit.get()->getPm();
                    int minX = unit.get()->getX() - unit.get()->getPm();
                    int maxY = unit.get()->getY() + unit.get()->getPm();
                    int minY = unit.get()->getY() - unit.get()->getPm();
                    std::uniform_int_distribution<int> distMoveX(minX, maxX);
                    std::uniform_int_distribution<int> distMoveY(minY, maxY);
                    int randomMoveX = distMoveX(mt);
                    int randomMoveY = distMoveY(mt);
                    Node depart = {.x =  unit.get()->getX(), .y = unit.get()->getY()};
                    Node destination = {.x = randomMoveX, .y = randomMoveY};
                    vector<Node> trajectory = Cordinate::aStar(depart, destination, e.getGameState().getWorld(),
                                                               e.getGameState().getGameObjects(), unit.get()->getPm());
                    if (trajectory.size() > 1) {
                        cout << "random move x" << randomMoveX << endl;
                        cout << "random move y" << randomMoveY << endl;
                        shared_ptr<Command> move = make_shared<MoveCommand>(unit, randomMoveX, randomMoveY);
                        e.addCommand(move, 1);
                        e.runCommands();
                        break;
                    } else {
                        cout << "AI didn't find a path to target, it tries again" << endl;
                    }
                }
            } else if (((randomValue - 2.0) * (randomValue - 4.0) < 0.0) & !units.getHasAttacked()) {
                while (!units.getHasAttacked()) {
                    int maxX = unit.get()->getX() + unit.get()->getWeapon().getRangeMax();
                    int minX = unit.get()->getX() - unit.get()->getWeapon().getRangeMax();
                    int maxY = unit.get()->getY() + unit.get()->getWeapon().getRangeMax();
                    int minY = unit.get()->getY() - unit.get()->getWeapon().getRangeMax();
                    std::uniform_int_distribution<int> distAttackX(minX, maxX);
                    std::uniform_int_distribution<int> distAttackY(minY, maxY);
                    int randomAttackX = distAttackX(mt);
                    int randomAttackY = distAttackY(mt);
                    vector<int> attackField = DisplayAttack::createField(unit.get(), e.getGameState().getWorld());
                    if (attackField[randomAttackX + randomAttackY * e.getGameState().getWorld().getYMax()] == 1) {
                        cout << "random attack x" << randomAttackX << endl;
                        cout << "random attack y" << randomAttackY << endl;
                        shared_ptr<Command> attack = make_shared<AttackCommand>(unit, randomAttackX, randomAttackY);
                        e.addCommand(attack, 1);
                        e.runCommands();
                        e.getGameState().unselectedUnit();
                        units.setHasAttacked(true); // TODO: quick fix; need solution
                    } else {
                        cout << "Unit can't attack target, it tries again" << endl;
                    }
                }
            } else if (4.0 < randomValue) {
                cout << "AI is moving to another unit" << endl;
                break;
            }
        }
    }
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands();
}

