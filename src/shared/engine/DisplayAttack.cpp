#include <vector>
#include <cmath>
#include "state.h"
#include "Node.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace state;

typedef unsigned int uint;

static int multipliers[4][8] = {
        {1, 0, 0, -1, -1, 0, 0, 1},
        {0, 1, -1, 0, 0, -1, 1, 0},
        {0, 1, 1, 0, 0, -1, -1, 0},
        {1, 0, 0, 1, -1, 0, 0, -1}
};

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

    static double calculateH(int x, int y, int destx, int desty) {
        return sqrt(((destx-x)^2) + ((desty-y)^2));
    }

    static vector<int> findCorrectBlock(int x, int y, Character *unit, const World& world, vector<int> field, int min){
        int posx = unit->getX() + x;
        int posy = unit->getY() + y;
        if(isValid(posx, posy, world)){
            if (abs(x) + abs(y) > min){
                field.at(posx + posy * world.getYMax()) = 1;
                // do_fov(field, posx, posy, calculateH(unit->getX(), unit->getY(), posx, posy), world);
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

    static void cast_light(vector<int>& field, uint x, uint y, uint radius, uint row,
                           float start_slope, float end_slope, uint xx, uint xy, uint yx,
                           uint yy, const World& world) {
        if (start_slope < end_slope) {
            return;
        }
        float next_start_slope = start_slope;
        for (uint i = row; i <= radius; i++) {
            bool blocked = false;
            for (int dx = -i, dy = -i; dx <= 0; dx++) {
                float l_slope = (dx - 0.5) / (dy + 0.5);
                float r_slope = (dx + 0.5) / (dy - 0.5);
                if (start_slope < r_slope) {
                    continue;
                } else if (end_slope > l_slope) {
                    break;
                }

                int sax = dx * xx + dy * xy;
                int say = dx * yx + dy * yy;
                if ((sax < 0 && (uint)std::abs(sax) > x) ||
                    (say < 0 && (uint)std::abs(say) > y)) {
                    continue;
                }
                uint ax = x + sax;
                uint ay = y + say;
                if (ax >= 20 || ay >= 20) {
                    continue;
                }

                uint radius2 = radius * radius;
                if ((uint)(dx * dx + dy * dy) < radius2) {
                    field.at(ax + ay * world.getYMax()) = 0;
                }

                if (blocked) {
                    if (isValid(ax, ay, world)) {
                        next_start_slope = r_slope;
                        continue;
                    } else {
                        blocked = false;
                        start_slope = next_start_slope;
                    }
                } else if (isValid(ax, ay, world)) {
                    blocked = true;
                    next_start_slope = r_slope;
                    cast_light(field, x, y, radius, i + 1, start_slope, l_slope, xx,
                               xy, yx, yy, world);
                }
            }
            if (blocked) {
                break;
            }
        }
    }

    static void do_fov(vector<int>& field, uint x, uint y, uint radius, const World& world) {
        for (uint i = 0; i < 8; i++) {
            cast_light(field, x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
                       multipliers[1][i], multipliers[2][i], multipliers[3][i], world);
        }
    }
};

