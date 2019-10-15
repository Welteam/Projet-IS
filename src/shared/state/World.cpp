#include "World.h"

namespace state {

    World::World() {

    }

    World::World(std::string fileName) {

    }

    std::vector<TileType> World::getTiles() {
        return this->tiles;
    }

    int World::getXMax() {
        return this->xMax;
    }

    int World::getXStep() {
        return this->xStep;
    }

    int World::getYMax() {
        return this->yMax;
    }

    int World::getYStep() {
        return this->yStep;
    }

    std::vector<GameObject> World::drawPattern(Character character, GameObject destination) {
        return std::vector<GameObject>();
    }

    std::vector<Character> World::getSpawnUnits1() {
        return this->spawnUnits1;
    }

    std::vector<Character> World::getSpawnUnits2() {
        return this->spawnUnits2;
    }

    std::vector<Tower> World::getSpawnTowers1() {
        return this->spawnTowers1;
    }

    std::vector<Tower> World::getSpawnTowers2() {
        return this->spawnTowers2;
    }

    std::vector<ApparitionArea> World::getSpawnApparitionAreas1() {
        return this->spawnApparitionAreas1;
    }

    std::vector<ApparitionArea> World::getSpawnApparitionAreas2() {
        return this->spawnApparitionAreas2;
    }

}

