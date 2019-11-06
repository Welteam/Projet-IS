#include <vector>
#include <cmath>
#include <iostream>
#include "state.h"


using namespace std;
using namespace state;

class DisplayAttack {
public:


    static bool isValid(int x, int y, state::World world) { //If our Node is an obstacle it is not valid
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

    static vector<int> findCorrectBlock(int x, int y, Character *unit, World world, vector<int> field, int min){
        int posx = unit->getX() + x;
        int posy = unit->getY() + y;
        if(isValid(posx, posy, world)){
            try {
                field.at(posx + posy * world.getYMax()) = 1;
            } catch (const exception &e) {
                cout <<e.what() << endl;
            }
        }
        field.at(unit->getX() + unit->getY() * world.getYMax()) = 0;
        return field;
    }


    static vector<int> createField(Character *unit, World world) {
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
};

