#include "World.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>

namespace state {

    World::World() : xMax(0), yMax(0) {

    }

    World::World(std::string fileName) {

        std::ifstream t(fileName);
        if(!t.good())
            throw std::runtime_error("Cannot find file to load map");

        std::string str;
        t.seekg(0, std::ios::end);
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        str.assign((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());

        std::vector<std::string> elements;
        std::vector<std::string> player1Elements;
        std::vector<std::string> player2Elements;
        std::vector<int> units1Pos;
        std::vector<int> towers1Pos;
        std::vector<int> apparitionAreas1Pos;
        std::vector<int> units2Pos;
        std::vector<int> towers2Pos;
        std::vector<int> apparitionAreas2Pos;
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
            spawnUnits1.push_back(character);
        }

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
            spawnTowers1.push_back(tower);
        }

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
            spawnApparitionAreas1.push_back(apparitionArea);
        }

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
            spawnUnits2.push_back(character);
        }

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
            spawnTowers2.push_back(tower);
        }

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
            spawnApparitionAreas2.push_back(apparitionArea);
        }

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

}

