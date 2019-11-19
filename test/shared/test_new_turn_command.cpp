#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/shared/engine.h"
#include "../../src/shared/state.h"

using namespace engine;
using namespace std;
using namespace state;

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestNewTurnCommand)
{
  {
      // 1. Intancier GameState
      GameState gameState{};

      // 2. Charger la carte World dans GameState
      gameState.setWorld(World{"../../../res/map.txt"});

      // 3. Charger players dans GameState
      gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
      gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
      gameState.setActivePlayer(gameState.getPlayer1());

      // 3. Associer gameState à engine
      shared_ptr<Engine> engine = make_shared<Engine>(gameState);

      // 4. Chercher une unité présente dans le player active qui a pour
      // coordonnée le x et y choisi.
      int x = 2;
      int y = 2;
      for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
          if(unit.getX() == x && unit.getY() == y){
              engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
          }
      }
      shared_ptr<Command> move = make_shared<MoveCommand>(engine->getGameState().getSelectedUnit(), 4, 0);
      engine->addCommand(move, 1);
      engine->runCommands(true);
      BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(2).getX(), 4);
      BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(2).getY(), 0);
      BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(2).getPm(), 1);
      shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
      engine->addCommand(newTurnCommand, 1);
      engine->runCommands(true);
      BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(2).getPm(), 5);
  }
}

BOOST_AUTO_TEST_CASE(TestNewTurnCommandPLayer2)
{
    {
        // 1. Intancier GameState
        GameState gameState{};

        // 2. Charger la carte World dans GameState
        gameState.setWorld(World{"../../../res/map.txt"});

        // 3. Charger players dans GameState
        gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
        gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
        gameState.setActivePlayer(gameState.getPlayer2());

        // 3. Associer gameState à engine
        shared_ptr<Engine> engine = make_shared<Engine>(gameState);

        // 4. Chercher une unité présente dans le player active qui a pour
        // coordonnée le x et y choisi.
        int x = 3;
        int y = 9;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
            }
        }
        shared_ptr<Command> move = make_shared<MoveCommand>(engine->getGameState().getSelectedUnit(), 0, 11);
        engine->addCommand(move, 1);
        engine->runCommands(true);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getX(), 0);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getY(), 11);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getPm(), 0);
        shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
        engine->addCommand(newTurnCommand, 1);
        engine->runCommands(true);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getPm(), 5);
    }
}

/* vim: set sw=2 sts=2 et : */
