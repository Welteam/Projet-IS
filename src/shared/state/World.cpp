#include "World.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace state;
using namespace std;

World::World() {
    this->xMax = 0;
    this->yMax = 0;
}

World::World(const string& fileName) {
    ifstream t(fileName);
    if(!t.good())
        throw runtime_error("Cannot find file to load map");
    string str;
    t.seekg(0, ios::end);
    str.reserve(t.tellg());
    t.seekg(0, ios::beg);
    str.assign((istreambuf_iterator<char>(t)),
            istreambuf_iterator<char>());
    vector<string> elements;
    vector<string> player1Elements;
    vector<string> player2Elements;
    stringstream ss(str);
    vector<int> xY;
    // Separate data to build World and Players
    while(ss.good())
    {
        string substr;
        getline( ss, substr, ':' );
        elements.push_back(substr );
    }
    // Transform string tiles into vector<TileType>
    stringstream sTiles(elements.at(0));
    for (int i = 0; sTiles >> i;) {
        tiles.push_back(static_cast<TileType>(i));
        while (sTiles.peek() == ',' || sTiles.peek() == ' ')
            sTiles.ignore();
    }
    // Transform string player 1 into units, towers, apparitionAreas
    stringstream sPlayer1(elements.at(1));
    while(sPlayer1.good())
    {
        string substr;
        getline( sPlayer1, substr, '/' );
        player1Elements.push_back(substr);
    }
    // Transform string player 2 into units, towers, apparitionAreas
    stringstream sPlayer2(elements.at(2));
    while(sPlayer2.good())
    {
        string substr;
        getline( sPlayer2, substr, '/' );
        player2Elements.push_back(substr);
    }
    // Transform units string into object
    for(auto gameObject : transformStringToPlayersObject(player1Elements.at(0), Character{}))
        spawnUnits1.emplace_back(gameObject.getX(), gameObject.getY());
    // Transform towers string into object
    for(auto gameObject : transformStringToPlayersObject(player1Elements.at(1), Tower{}))
        spawnTowers1.emplace_back(gameObject.getX(), gameObject.getY());
    // Transform apparition areas string into object
    for(auto gameObject : transformStringToPlayersObject(player1Elements.at(2), ApparitionArea{}))
        spawnApparitionAreas1.emplace_back(gameObject.getX(), gameObject.getY());
    // Transform units string into object
    for(auto gameObject : transformStringToPlayersObject(player2Elements.at(0), Character{}))
        spawnUnits2.emplace_back(gameObject.getX(), gameObject.getY());
    // Transform towers string into object
    for(auto gameObject : transformStringToPlayersObject(player2Elements.at(1), Tower{}))
        spawnTowers2.emplace_back(gameObject.getX(), gameObject.getY());
    // Transform apparition areas string into object
    for(auto gameObject : transformStringToPlayersObject(player2Elements.at(2), ApparitionArea{}))
        spawnApparitionAreas2.emplace_back(gameObject.getX(), gameObject.getY());
    // Transform string X Y into variable
    stringstream sXY(elements.at(3));
    for (int i = 0; sXY >> i;) {
        xY.push_back(static_cast<TileType>(i));
        while (sXY.peek() == ',' || sXY.peek() == ' ')
            sXY.ignore();
    }
    this->xMax = xY.at(0);
    this->yMax = xY.at(1);
}

vector<TileType> World::getTiles() const {
    return this->tiles;
}

int World::getXMax() const {
    return this->xMax;
}

int World::getYMax() const {
    return this->yMax;
}

vector<Character> World::getSpawnUnits1() const {
    return this->spawnUnits1;
}

vector<Character> World::getSpawnUnits2() const {
    return this->spawnUnits2;
}

vector<Tower> World::getSpawnTowers1() const {
    return this->spawnTowers1;
}

vector<Tower> World::getSpawnTowers2() const {
    return this->spawnTowers2;
}

vector<ApparitionArea> World::getSpawnApparitionAreas1() const {
    return this->spawnApparitionAreas1;
}

vector<ApparitionArea> World::getSpawnApparitionAreas2() const {
    return this->spawnApparitionAreas2;
}

vector<GameObject> World::transformStringToPlayersObject(const string& strGameObject, GameObject gameObject){
    // Transform units string into object
    vector<int> gameObjectPos;
    stringstream ss(strGameObject);
    vector<GameObject> gameObjects;
    for (int i = 0; ss >> i;) {
        gameObjectPos.push_back(i);
        while (ss.peek() == ',' || ss.peek() == ' ')
            ss.ignore();
    }
    for(ulong i = 0; i < gameObjectPos.size(); i += 2){
        gameObject.setX(gameObjectPos[i]);
        gameObject.setY(gameObjectPos[i + 1]);
        gameObjects.push_back(gameObject);
    }
    return gameObjects;
}
