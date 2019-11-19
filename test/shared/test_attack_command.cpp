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

BOOST_AUTO_TEST_CASE(TestAttackCommandConstructor)
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
      int x = 1;
      int y = 2;
      for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
          if(unit.getX() == x && unit.getY() == y){
              engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
          }
      }
    AttackCommand attackCommand{engine->getGameState().getSelectedUnit(), 5, 4};
    BOOST_CHECK_EQUAL(attackCommand.selectedUnit.get()->getX(), 1);
    BOOST_CHECK_EQUAL(attackCommand.selectedUnit.get()->getY(), 2);
    BOOST_CHECK_EQUAL(attackCommand.target.x, 5);
    BOOST_CHECK_EQUAL(attackCommand.target.y, 4);
  }
}

BOOST_AUTO_TEST_CASE(TestAttackCommandExecutePlayer2)
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
        int x = 1;
        int y = 5;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
            }
        }
        shared_ptr<Command> attackCommand = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 1, 2);
        engine->addCommand(attackCommand, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(0).getHp(), 70);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(1).getHp(), 70);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(2).getHp(), 70);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().at(3).getHp(), 100);
        shared_ptr<Command> attackCommand2 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 1, 2);
        engine->addCommand(attackCommand2, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand3 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 1, 2);
        engine->addCommand(attackCommand3, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand4 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 1, 2);
        engine->addCommand(attackCommand4, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getUnits().size(), 1);
        BOOST_CHECK_EQUAL(engine->getGameState().getActivePlayer().getUnits().at(0).getWeapon().getLevel(), 3);
        // 4. Chercher une unité présente dans le player active qui a pour
        // coordonnée le x et y choisi.
        x = 3;
        y = 9;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
            }
        }
        shared_ptr<Command> attackCommand5 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getTowers().at(0).getHp(), 70);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getTowers().at(0).getHp(), 70);
    }
}

BOOST_AUTO_TEST_CASE(TestAttackCommandExecutePlayer1)
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
        int x = 3;
        int y = 12;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
            }
        }
        shared_ptr<Command> attackCommand = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 9);
        engine->addCommand(attackCommand, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getHp(), 70);
        shared_ptr<Command> attackCommand2 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 9);
        engine->addCommand(attackCommand2, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand3 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 9);
        engine->addCommand(attackCommand3, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand4 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 9);
        engine->addCommand(attackCommand4, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().size(), 3);
        BOOST_CHECK_EQUAL(engine->getGameState().getActivePlayer().getUnits().at(3).getWeapon().getLevel(), 1);
    }
}
BOOST_AUTO_TEST_CASE(TestAttackCommandExecutePlayer1Towers)
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

        int x = 3;
        int y = 12;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
            }
        }
        shared_ptr<Command> attackCommand5 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getTowers().at(0).getHp(), 70);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getTowers().at(0).getHp(), 70);
        shared_ptr<Command> attackCommand6 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand7 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand8 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getTowers().size(), 0);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getTowers().size(), 0);
        BOOST_CHECK_EQUAL(engine->getGameState().getActivePlayer().getUnits().at(3).getWeapon().getLevel(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestAttackCommandExecutePlayer2Towers)
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

        int x = 3;
        int y = 9;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
            }
        }
        shared_ptr<Command> attackCommand5 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getTowers().at(0).getHp(), 70);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getTowers().at(0).getHp(), 70);
        shared_ptr<Command> attackCommand6 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand7 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        shared_ptr<Command> attackCommand8 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 8, 10);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer1().getTowers().size(), 0);
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getTowers().size(), 0);
        BOOST_CHECK_EQUAL(engine->getGameState().getActivePlayer().getUnits().at(1).getWeapon().getLevel(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestAttackCommandExecutePlayerOrientation)
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
        shared_ptr<Command> attackCommand1 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 6);
        engine->addCommand(attackCommand1, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::NORTH);
        shared_ptr<Command> attackCommand2 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 12);
        engine->addCommand(attackCommand2, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::SOUTH);
        shared_ptr<Command> attackCommand3 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 0, 9);
        engine->addCommand(attackCommand3, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::WEST);
        shared_ptr<Command> attackCommand4 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 6, 9);
        engine->addCommand(attackCommand4, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::EST);
        shared_ptr<Command> attackCommand5 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 4, 9);
        engine->addCommand(attackCommand5, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::EST);
        shared_ptr<Command> attackCommand6 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 8);
        engine->addCommand(attackCommand6, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::NORTH);
        shared_ptr<Command> attackCommand7 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 3, 11);
        engine->addCommand(attackCommand7, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::SOUTH);
        shared_ptr<Command> attackCommand8 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 2, 9);
        engine->addCommand(attackCommand8, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::WEST);

        shared_ptr<Command> attackCommand9 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 2, 10);
        engine->addCommand(attackCommand9, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::WEST);

        shared_ptr<Command> attackCommand10 = make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(), 5, 11);
        engine->addCommand(attackCommand10, 1);
        engine->runCommands();
        BOOST_CHECK_EQUAL(engine->getGameState().getPlayer2().getUnits().at(1).getOrientation(), Orientation::EST);

    }
}



/* vim: set sw=2 sts=2 et : */
