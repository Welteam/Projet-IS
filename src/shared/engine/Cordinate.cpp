#include <stack>
#include <vector>
#include <array>
#include <cmath>
#include <cfloat>
#include <iostream>
#include "../engine.h"

#define X_MAX 320
#define X_STEP 16
#define Y_MAX 320
#define Y_STEP 16

using namespace std;
using namespace engine;

inline bool operator < (const Node& lhs, const Node& rhs)
{//We need to overload "<" to put our struct into a set
    return lhs.fCost < rhs.fCost;
}


class Cordinate {
public:


    static bool isValid(int x, int y, state::World world, vector<state::GameObject> gameObjects) { //If our Node is an obstacle it is not valid
        int id = x + y * 20;
        for(auto gameObject : gameObjects){
            if(gameObject.getX() == x && gameObject.getY() == y){
                cout << "La destination est obstruée par un objet"<< endl;
                cout << "L'objet est en x = " << gameObject.getX() << " et y = " << gameObject.getY() << endl;
                return false;
            }
        }
        if (x < 0 || y < 0 || x > (X_MAX / X_STEP)-1 || y > (Y_MAX / Y_STEP)-1) {
            cout << "en dehors de la map"<< endl;
            return false;
        }
        if (world.getTiles().at(id) == 0 || world.getTiles().at(id) == 3) {
            return true;
        }
        cout << "obstacle de map en x = "<< x << " et y = " << y << endl;
        return false;
    }

    static bool isDestination(int x, int y, Node dest) {
        if (x == dest.x && y == dest.y) {
            return true;
        }
        return false;
    }

    static double calculateH(int x, int y, Node dest) {
        int dx = abs(x - dest.x);
        int dy = abs(y - dest.y);
        return 1 * (dx + dy);
    }

    static vector<Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest) {
        int x = dest.x;
        int y = dest.y;
        stack<Node> path;
        vector<Node> usablePath;
            while (!(map[x][y].parentX == x && map[x][y].parentY == y)
                   && map[x][y].x != -1 && map[x][y].y != -1) {
                path.push(map[x][y]);
                int tempX = map[x][y].parentX;
                int tempY = map[x][y].parentY;
                x = tempX;
                y = tempY;

            }
            path.push(map[x][y]);

            while (!path.empty()) {
                Node top = path.top();
                path.pop();
                usablePath.emplace_back(top);
            }
        return usablePath;
    }


    static vector<Node> aStar(Node player, Node dest, state::World world, vector<state::GameObject> gameObjectsWithUnitSelected, int pm) {
        vector<Node> empty;
        empty.push_back(player);
        vector<state::GameObject> gameObjects;
        for(auto gameObject : gameObjectsWithUnitSelected){
            if(gameObject.getX() != player.x || gameObject.getY() != player.y)
                gameObjects.push_back(gameObject);
        }
        if (isValid(dest.x, dest.y, world, gameObjects) == false) {
            cout << "Destination is invalid"<< endl;
            return empty;
            //Destination is invalid
        }
        if (isDestination(player.x, player.y, dest)) {
            cout << "You clicked on yourself"<< endl;
            return empty;
            //You clicked on yourself
        }
        bool closedList[(X_MAX / X_STEP)][(Y_MAX / Y_STEP)];

        //Initialize whole map
        //Node allMap[50][25];
        array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> allMap;
        for (int x = 0; x < (X_MAX / X_STEP); x++) {
            for (int y = 0; y < (Y_MAX / Y_STEP); y++) {
                allMap[x][y].fCost = FLT_MAX;
                allMap[x][y].gCost = FLT_MAX;
                allMap[x][y].hCost = FLT_MAX;
                allMap[x][y].parentX = -1;
                allMap[x][y].parentY = -1;
                allMap[x][y].x = x;
                allMap[x][y].y = y;

                closedList[x][y] = false;
            }
        }

        //Initialize our starting list
        int x = player.x;
        int y = player.y;
        allMap[x][y].fCost = 0.0;
        allMap[x][y].gCost = 0.0;
        allMap[x][y].hCost = 0.0;
        allMap[x][y].parentX = x;
        allMap[x][y].parentY = y;

        vector<Node> openList;
        openList.emplace_back(allMap[x][y]);
        //bool destinationFound = false;

        while (!openList.empty() && openList.size() < (X_MAX / X_STEP) * (Y_MAX / Y_STEP)) {
            Node node;
            do {
                //This do-while loop could be replaced with extracting the first
                //element from a set, but you'd have to make the openList a set.
                //To be completely honest, I don't remember the reason why I do
                //it with a vector, but for now it's still an option, although
                //not as good as a set performance wise.
                float temp = FLT_MAX;
                vector<Node>::iterator itNode;
                for (vector<Node>::iterator it = openList.begin();
                     it != openList.end(); it = next(it)) {
                    Node n = *it;
                    if (n.fCost < temp) {
                        temp = n.fCost;
                        itNode = it;
                    }
                }
                node = *itNode;
                openList.erase(itNode);
            } while (isValid(node.x, node.y, world, gameObjects) == false);
                x = node.x;
                y = node.y;
                closedList[x][y] = true;


            //For each neighbour starting from North-West to South-East
            vector<pair<int, int>> fourWay;
            fourWay.push_back(pair<int, int>(-1, 0));
            fourWay.push_back(pair<int, int>(0, 0));
            fourWay.push_back(pair<int, int>(0, -1));
            fourWay.push_back(pair<int, int>(0, 0));
            fourWay.push_back(pair<int, int>(0, 1));
            fourWay.push_back(pair<int, int>(0, 0));
            fourWay.push_back(pair<int, int>(1, 0));
            fourWay.push_back(pair<int, int>(0, 0));

            for (auto coord : fourWay) {
                int newX = coord.first;
                int newY = coord.second;
                if(y + newY < 0 || y + newY > (Y_MAX / Y_STEP)-1){
                    newY = 0;
                }
                if(x + newX < 0 || x + newX > (X_MAX / X_STEP)-1){
                    newX = 0;
                }
                double gNew, hNew, fNew;
                if (isValid(x + newX, y + newY, world, gameObjects)) {
                    if (isDestination(x + newX, y + newY, dest)) {
                        //Destination found - make path
                        allMap[x + newX][y + newY].parentX = x;
                        allMap[x + newX][y + newY].parentY = y;
                        //destinationFound = true;
                        if(makePath(allMap, dest).size() > static_cast<unsigned int>(pm+1)){
                            return empty;
                        } else {
                            return makePath(allMap, dest);
                        }
                    } else if (closedList[x + newX][y + newY] == false) {
                        gNew = node.gCost + 1.0;
                        hNew = calculateH(x + newX, y + newY, dest);
                        fNew = gNew + hNew;
                        // Check if this path is better than the one already present
                        if (allMap[x + newX][y + newY].fCost == FLT_MAX ||
                            allMap[x + newX][y + newY].fCost > fNew) {
                            // Update the details of this neighbour node
                            allMap[x + newX][y + newY].fCost = fNew;
                            allMap[x + newX][y + newY].gCost = gNew;
                            allMap[x + newX][y + newY].hCost = hNew;
                            allMap[x + newX][y + newY].parentX = x;
                            allMap[x + newX][y + newY].parentY = y;

                            openList.emplace_back(allMap[x + newX][y + newY]);
                        }
                    }
                }

            }
        }
        /*if (destinationFound == false)
            return empty;*/
        return empty;
    }

};


