#include <iostream>
#include <vector>
#include "MoveCommand.h"
#include "Cordinate.cpp"


namespace engine {

    MoveCommand::MoveCommand(std::shared_ptr<state::Character> selectedUnit, int newX, int newY) {
        this->selectedUnit = selectedUnit;
        this->destination = Node{.x = newX, .y = newY};
    }

    void MoveCommand::execute(state::GameState &gameState) {
        std::cout << "unité se déplace" << std::endl;
        engine::Node depart = {.x = selectedUnit->getX(), .y = selectedUnit->getY()};
        vector<Node> nodes = Cordinate::aStar(depart, destination, gameState.getWorld(), gameState.getGameObjects(), selectedUnit.get()->getWeapon().getPmMax());
        if(nodes.at(nodes.size()-1).x == destination.x && nodes.at(nodes.size()-1).y == destination.y){
            state::Player player = gameState.getActivePlayer();
            vector<state::Character> newUnits;
            for(auto unit : player.getUnits()){
                if(unit.getX() == selectedUnit->getX() && unit.getY() == selectedUnit->getY()){
                    unit.setX(nodes.at(nodes.size()-1).x);
                    unit.setY(nodes.at(nodes.size()-1).y);
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
