#include <iostream>
#include <memory>
#include <zconf.h>
#include "engine.h"
#include "ai.h"
#include "Server.h"
#include "state.h"

using namespace engine;
using namespace server;
using namespace state;
using namespace std;
using namespace ai;


void Server::run() {

    /// 1. Intancier GameState
    GameState gameState{};
    /// 5. Charger la carte World dans GameState
    gameState.setWorld(World{"../res/map2.txt"});
    /// 6. Charger players dans GameState
    gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
    gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
    gameState.setActivePlayer(gameState.getPlayer1());
    /// 7. Create engine
    shared_ptr<Engine> engine = make_shared<Engine>(gameState);

    // Call our AI computer
    shared_ptr<AI> aiTest;
    aiTest.reset(new HeuristicAI);
    aiTest->restrictArea = false;
    unsigned int count = 0;

    while(count < 2 ) {
        aiTest->run(*engine);
        cout << "about to sleep" << endl;
        usleep(1000000);
        cout << "end commands" << endl;
        //}
        cout << "epoch : " << count <<endl;
        count ++;
    }
}


