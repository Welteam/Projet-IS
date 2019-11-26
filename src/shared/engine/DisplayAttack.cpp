#include <vector>
#include <cmath>
#include "state.h"
#include "Node.h"
#include <iostream>

using namespace std;
using namespace state;

class DisplayAttack {
public:


    static bool isValid(int x, int y, const state::World& world) { //If our Node is an obstacle it is not valid
        int id = x + y * world.getYMax();
        if (x < 0 || y < 0 || x >= world.getXMax() || y >= world.getYMax()) {
            return false;
        } else {
            if (world.getTiles().at(id) == 0 || world.getTiles().at(id) == 3) {
                return true;
            }
        }
        return false;
    }

    static vector<int> findCorrectBlock(int x, int y, Character *unit, const World& world, vector<int> field, int min){
        int posx = unit->getX() + x;
        int posy = unit->getY() + y;
        if(isValid(posx, posy, world)){
            if (abs(x) + abs(y) > min){
                if(isValidBetweenPositionAndTarget(unit->getX(), unit->getY(), posx, posy, world)){
                    field.at(posx + posy * world.getYMax()) = 1;
                } else {
                    field.at(posx + posy * world.getYMax()) = 0;
                }
            }
        }
        return field;
    }

    static vector<int> createField(Character *unit, const World& world) {
        // Define principles variables
        int min = unit->getWeapon().getRangeMin();
        int max = unit->getWeapon().getRangeMax();
        // Initialize at 0 to make transparent
        vector<int> field(world.getXMax()*world.getYMax(), 0);
        for(int x = -max; x <= max; x++){
            for(int y = -max; y <= max; y++) {
                switch (unit->getWeapon().getDirection()) {
                    case DirectionType::FULL:
                        if (abs(x) + abs(y) <= max) {
                            field = findCorrectBlock(x, y, unit, world, field, min);
                        }
                        break;
                    case DirectionType::DIAGONAL:
                        if (abs(x) == abs(y)) {
                            field = findCorrectBlock(x, y, unit, world, field, min);
                        }
                        break;
                    case DirectionType::CROSS:
                        if (x == 0 || y == 0) {
                            field = findCorrectBlock(x, y, unit, world, field, min);
                        }
                        break;
                    case DirectionType::OCTOPUS:
                        if (x == 0 || y == 0 || abs(x) == abs(y)) {
                            field = findCorrectBlock(x, y, unit, world, field, min);
                        }
                        break;
                }
            }
        }
        return field;
    }

    static vector<engine::Node> createDamageArea(int mouseX, int mouseY, Character *unit, const World& world) {
        // Define principles variables
        int max = unit->getWeapon().getDamageAreaMax();
        vector<engine::Node> damageArea;

        for(int x = -max; x <= max; x++) {
            for (int y = -max; y <= max; y++) {
                if (abs(x) + abs(y) <= max) {
                    int posx = mouseX + x;
                    int posy = mouseY + y;
                    if(isValid(posx, posy, world)){
                        engine::Node node = {.x = posx, .y = posy};
                        damageArea.push_back(node);
                    }
                }
            }
        }
        return  damageArea;
    }

    static bool isValidBetweenPositionAndTarget(int pX, int pY, int tX, int tY, const World& world){
        double arct = atan2((tY - pY), (tX - pX));
        double distEucli = sqrt((tX - pX) * (tX - pX) + (tY - pY) * (tY - pY));
        int oldX = 0;
        int oldY = 0;
        for(int i = 0; i < distEucli*16; i++){
            int pixX = static_cast<int>(round(posToPix(pX)+i*cos(arct)));
            int pixY = static_cast<int>(round(posToPix(pY)+i*sin(arct)));
            if(oldX != pixToPos(pixX) || oldY != pixToPos(pixY)){
                if(!isValid(pixToPos(pixX), pixToPos(pixY), world)){
                    return false;
                }
                oldX = pixToPos(pixX);
                oldY = pixToPos(pixY);
            }
        }
        return true;
    }

    static int posToPix(int pos){
        return 8 + 16*pos;
    }

    static int pixToPos(int pixel){
        return static_cast<int>(round((static_cast<double >(pixel)-8.0)/16.0));
    }

};

