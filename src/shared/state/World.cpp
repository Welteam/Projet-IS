#include "World.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

namespace state {

    World::World() : xMax(0), yMax(0) {

    }

    World::World(std::string fileName) {

        this->xMax = 20;
        this->yMax = 20;

        std::string str = fileName;
        std::vector<std::string> elements;
        std::vector<std::string> player1Elements;
        std::vector<std::string> player2Elements;
        std::vector<TileType> tiles;
        std::vector<int> units1Pos;
        std::vector<Character> units1;
        std::vector<int> towers1Pos;
        std::vector<Tower> towers1;
        std::vector<int> apparitionAreas1Pos;
        std::vector<ApparitionArea> apparitionAreas1;
        std::vector<int> units2Pos;
        std::vector<Character> units2;
        std::vector<int> towers2Pos;
        std::vector<Tower> towers2;
        std::vector<int> apparitionAreas2Pos;
        std::vector<ApparitionArea> apparitionAreas2;
        std::stringstream ss(str);
        std::vector<int> xY;



        // Separate data to build World and Players
        while(ss.good())
        {
            std::string substr;
            getline( ss, substr, ':' );
            elements.push_back(substr );
        }



        // Transform string tiles into vector<TileType>
        std::stringstream sTiles(elements.at(0));
        for (int i = 0; sTiles >> i;) {
            tiles.push_back(static_cast<TileType>(i));
            while (sTiles.peek() == ',' || sTiles.peek() == ' ')
                sTiles.ignore();
        }
        this->tiles = tiles;



        // Transform string player 1 into units, towers, apparitionAreas
        std::stringstream sPlayer1(elements.at(1));
        while(sPlayer1.good())
        {
            std::string substr;
            getline( sPlayer1, substr, '/' );
            player1Elements.push_back(substr);
        }

        // Transform units string into object
        std::stringstream sUnits1(player1Elements.at(0));
        for (int i = 0; sUnits1 >> i;) {
            units1Pos.push_back(i);
            while (sUnits1.peek() == ',' || sUnits1.peek() == ' ')
                sUnits1.ignore();
        }
        for(uint i = 0; i < units1Pos.size(); i += 2){
            Character character;
            character.setX(units1Pos[i]);
            character.setY(units1Pos[i+1]);
            units1.push_back(character);
        }
        this->spawnUnits1 = units1;

        // Transform towers string into object
        std::stringstream sTowers1(player1Elements.at(1));
        for (int i = 0; sTowers1 >> i;) {
            towers1Pos.push_back(i);
            while (sTowers1.peek() == ',' || sTowers1.peek() == ' ')
                sTowers1.ignore();
        }
        for(uint i = 0; i < towers1Pos.size(); i += 2){
            Tower tower;
            tower.setX(towers1Pos[i]);
            tower.setY(towers1Pos[i+1]);
            towers1.push_back(tower);
        }
        this->spawnTowers1 = towers1;

        // Transform apparition areas string into object
        std::stringstream sApparitionAreas1(player1Elements.at(2));
        for (int i = 0; sApparitionAreas1 >> i;) {
            apparitionAreas1Pos.push_back(i);
            while (sApparitionAreas1.peek() == ',' || sApparitionAreas1.peek() == ' ')
                sApparitionAreas1.ignore();
        }
        for(uint i = 0; i < apparitionAreas1Pos.size(); i += 2){
            ApparitionArea apparitionArea;
            apparitionArea.setX(apparitionAreas1Pos[i]);
            apparitionArea.setY(apparitionAreas1Pos[i+1]);
            apparitionAreas1.push_back(apparitionArea);
        }
        this->spawnApparitionAreas1 = apparitionAreas1;



        // Transform string player 1 into units, towers, apparitionAreas
        std::stringstream sPlayer2(elements.at(2));
        while(sPlayer2.good())
        {
            std::string substr;
            getline( sPlayer2, substr, '/' );
            player2Elements.push_back(substr);
        }

        // Transform units string into object
        std::stringstream sUnits2(player2Elements.at(0));
        for (int i = 0; sUnits2 >> i;) {
            units2Pos.push_back(i);
            while (sUnits2.peek() == ',' || sUnits2.peek() == ' ')
                sUnits2.ignore();
        }
        for(uint i = 0; i < units2Pos.size(); i += 2){
            Character character;
            character.setX(units2Pos[i]);
            character.setY(units2Pos[i+1]);
            units2.push_back(character);
        }
        this->spawnUnits2 = units2;

        // Transform towers string into object
        std::stringstream sTowers2(player2Elements.at(1));
        for (int i = 0; sTowers2 >> i;) {
            towers2Pos.push_back(i);
            while (sTowers2.peek() == ',' || sTowers2.peek() == ' ')
                sTowers2.ignore();
        }
        for(uint i = 0; i < towers2Pos.size(); i += 2){
            Tower tower;
            tower.setX(towers2Pos[i]);
            tower.setY(towers2Pos[i+1]);
            towers2.push_back(tower);
        }
        this->spawnTowers2 = towers2;

        // Transform apparition areas string into object
        std::stringstream sApparitionAreas2(player2Elements.at(2));
        for (int i = 0; sApparitionAreas2 >> i;) {
            apparitionAreas2Pos.push_back(i);
            while (sApparitionAreas2.peek() == ',' || sApparitionAreas2.peek() == ' ')
                sApparitionAreas2.ignore();
        }
        for(uint i = 0; i < apparitionAreas2Pos.size(); i += 2){
            ApparitionArea apparitionArea;
            apparitionArea.setX(apparitionAreas2Pos[i]);
            apparitionArea.setY(apparitionAreas2Pos[i+1]);
            apparitionAreas2.push_back(apparitionArea);
        }
        this->spawnApparitionAreas2 = apparitionAreas2;

        // Transform string X Y into variable
        std::stringstream sXY(elements.at(3));
        for (int i = 0; sXY >> i;) {
            xY.push_back(static_cast<TileType>(i));
            while (sXY.peek() == ',' || sXY.peek() == ' ')
                sXY.ignore();
        }
        this->xMax = xY.at(0);
        this->yMax = xY.at(1);

    }


    std::vector<TileType> World::getTiles() const {
        return this->tiles;
    }

    int World::getXMax() const {
        return this->xMax;
    }

    int World::getYMax() const {
        return this->yMax;
    }

    std::vector<Character> World::getSpawnUnits1() const {
        return this->spawnUnits1;
    }

    std::vector<Character> World::getSpawnUnits2() const {
        return this->spawnUnits2;
    }

    std::vector<Tower> World::getSpawnTowers1() const {
        return this->spawnTowers1;
    }

    std::vector<Tower> World::getSpawnTowers2() const {
        return this->spawnTowers2;
    }

    std::vector<ApparitionArea> World::getSpawnApparitionAreas1() const {
        return this->spawnApparitionAreas1;
    }

    std::vector<ApparitionArea> World::getSpawnApparitionAreas2() const {
        return this->spawnApparitionAreas2;
    }

    std::string mapWorld = "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 4, 4, 4, 4, 4,"
                           "3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,:"
                           "1, 2, 3, 7, 1, 17, 3, 12,/ 7, 10,/ 0, 0,:"
                           "1, 5, 3, 9, 4, 17, 1, 12,/ 9, 10,/ 0, 19,:";


}

