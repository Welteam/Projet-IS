#include "World.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <functional>

namespace state {

    World::World() {
        this->xMax = 0;
        this->yMax = 0;

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

        // Transform string player 2 into units, towers, apparitionAreas
        std::stringstream sPlayer2(elements.at(2));
        while(sPlayer2.good())
        {
            std::string substr;
            getline( sPlayer2, substr, '/' );
            player2Elements.push_back(substr);
        }

        // Transform units string into object
        for(auto gameObject : transformStringToPlayersObject(player1Elements.at(0), Character{}))
            spawnUnits1.push_back(Character{gameObject.getX(), gameObject.getY()});

        // Transform towers string into object
        for(auto gameObject : transformStringToPlayersObject(player1Elements.at(1), Tower{}))
            spawnTowers1.push_back(Tower{gameObject.getX(), gameObject.getY()});

        // Transform apparition areas string into object
        for(auto gameObject : transformStringToPlayersObject(player1Elements.at(2), ApparitionArea{}))
            spawnApparitionAreas1.push_back(ApparitionArea{gameObject.getX(), gameObject.getY()});

        // Transform units string into object
        for(auto gameObject : transformStringToPlayersObject(player2Elements.at(0), Character{}))
            spawnUnits2.push_back(Character{gameObject.getX(), gameObject.getY()});

        // Transform towers string into object
        for(auto gameObject : transformStringToPlayersObject(player2Elements.at(1), Tower{}))
            spawnTowers2.push_back(Tower{gameObject.getX(), gameObject.getY()});

        // Transform apparition areas string into object
        for(auto gameObject : transformStringToPlayersObject(player2Elements.at(2), ApparitionArea{}))
            spawnApparitionAreas2.push_back(ApparitionArea{gameObject.getX(), gameObject.getY()});

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

    std::vector<GameObject> World::transformStringToPlayersObject(std::string strGameObject, GameObject gameObject){
        // Transform units string into object
        std::vector<int> gameObjectPos;
        std::stringstream ss(strGameObject);
        std::vector<GameObject> gameObjects;

        for (int i = 0; ss >> i;) {
            gameObjectPos.push_back(i);
            while (ss.peek() == ',' || ss.peek() == ' ')
                ss.ignore();
        }
        for(uint i = 0; i < gameObjectPos.size(); i += 2){
            gameObject.setX(gameObjectPos[i]);
            gameObject.setY(gameObjectPos[i + 1]);
            gameObjects.push_back(gameObject);
        }
        return gameObjects;
    }




}

