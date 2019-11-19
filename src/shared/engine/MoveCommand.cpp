#include <iostream>
#include <utility>
#include <vector>
#include "MoveCommand.h"
#include "Cordinate.cpp"
#include <chrono>
#include <thread>


namespace engine {

    MoveCommand::MoveCommand(std::shared_ptr<state::Character> selectedUnit, int newX, int newY) {
        this->selectedUnit = std::move(selectedUnit);
        this->destination = Node{.x = newX, .y = newY};
    }

    void MoveCommand::execute(state::GameState &gameState) {
        //std::cout << "unité se déplace" << std::endl;
        engine::Node depart = {.x = selectedUnit->getX(), .y = selectedUnit->getY()};
        vector<Node> nodes = Cordinate::aStar(depart, destination, gameState.getWorld(), gameState.getGameObjects(), selectedUnit.get()->getPm());
        if(nodes.at(nodes.size()-1).x == destination.x && nodes.at(nodes.size()-1).y == destination.y){
            Node lastMove = Node{.x = selectedUnit->getX(), .y = selectedUnit->getY()};
            for(auto node : nodes){
                state::Player player = gameState.getActivePlayer();
                vector<state::Character> newUnits;
                for(auto unit : player.getUnits()){
                    if(unit.getX() == lastMove.x && unit.getY() == lastMove.y){
                        if((unit.getX()-node.x)  == 1){
                            unit.setOrientation(state::Orientation::WEST);
                        } else if((unit.getX()-node.x)  == -1){
                            unit.setOrientation(state::Orientation::EST);
                        } else {
                            if((unit.getY()-node.y)  == 1){
                                unit.setOrientation(state::Orientation::NORTH);
                            } else if((unit.getY()-node.y)  == -1){
                                unit.setOrientation(state::Orientation::SOUTH);
                            }
                        }
                        unit.setX(node.x);
                        unit.setY(node.y);
                        lastMove = Node{.x = node.x, .y = node.y};
                        if(!(unit.getX() == selectedUnit->getX() && unit.getY() == selectedUnit->getY())){
                            unit.usePm(1);
                            std::this_thread::sleep_for(std::chrono::milliseconds(150));
                        }
                    }
                    newUnits.push_back(unit);
                }
                player.setUnits(newUnits);
                if(player.getId() == 1){
                    gameState.setPlayer1(player);
                    gameState.setActivePlayer(gameState.getPlayer1());
                } else {
                    gameState.setPlayer2(player);
                    gameState.setActivePlayer(gameState.getPlayer2());
                }
            }
        }
    }
}
