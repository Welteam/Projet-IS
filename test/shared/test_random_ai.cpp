
#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "engine.h"
#include "ai.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"
#include <cstring>
#include <thread>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

bool iaTurn = false;

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestRandomAI)
{
  {
      // 1. Intancier GameState
      GameState gameState{};

      // 5. Charger la carte World dans GameState
      gameState.setWorld(World{"../../../res/map2.txt"});

      // 6. Charger players dans GameState
      gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
      gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
      gameState.setActivePlayer(gameState.getPlayer1());
      unique_ptr<AI> ai;
      ai.reset(new RandomAI);
      // Create our engine
      shared_ptr<Engine> engine = make_shared<Engine>(gameState);
      ai->run(*engine);
  }


}

/* vim: set sw=2 sts=2 et : */
