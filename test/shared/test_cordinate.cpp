#include <boost/test/unit_test.hpp>
#include "../../src/shared/engine.h"
#include "../../src/shared/engine/Cordinate.cpp"
#include "../../src/shared/state.h"

using namespace engine;
using namespace std;
using namespace state;

BOOST_AUTO_TEST_CASE(TestCordinateDestinationIsAnotherUnit)
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
      // 4. Chercher une unité présente dans le player active qui a pour coordonnée le x et y choisi.
      int x = 1;
      int y = 2;
      for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
          if(unit.getX() == x && unit.getY() == y){
              engine->setSelectedUnit(make_shared<Character>(unit));
          }
      }
      Node depart = Node{.x = engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()};
      Node destination = Node{.x = 1, .y = 3};
      vector<Node> nodes = Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), engine->getGameState().getGameObjects(), engine->getSelectedUnit().get()->getPm());
      BOOST_CHECK_EQUAL(nodes.size(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TestCordinateDestinationIsAnElementOfTheMap)
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
        // 4. Chercher une unité présente dans le player active qui a pour coordonnée le x et y choisi.
        int x = 2;
        int y = 2;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->setSelectedUnit(make_shared<Character>(unit));
            }
        }
        Node depart = Node{.x = engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()};
        Node destination = Node{.x = 5, .y = 2};
        vector<Node> nodes = Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), engine->getGameState().getGameObjects(), engine->getSelectedUnit().get()->getPm());
        BOOST_CHECK_EQUAL(nodes.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestCordinateDestinationIsYourself)
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
        // 4. Chercher une unité présente dans le player active qui a pour coordonnée le x et y choisi.
        int x = 2;
        int y = 2;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->setSelectedUnit(make_shared<Character>(unit));
            }
        }
        Node depart = Node{.x = engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()};
        Node destination = Node{.x = 2, .y = 2};
        vector<Node> nodes = Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), engine->getGameState().getGameObjects(), engine->getSelectedUnit().get()->getPm());
        BOOST_CHECK_EQUAL(nodes.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestCordinateFuncIsValid)
{
    // 1. Intancier GameState
    GameState gameState{};
    // 2. Charger la carte World dans GameState
    gameState.setWorld(World{"../../../res/map.txt"});
    // 3. Charger players dans GameState
    gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
    gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
    gameState.setActivePlayer(gameState.getPlayer1());
    // X en dehors de la map, return false
    BOOST_CHECK_EQUAL(Cordinate::isValid(-1, 18, gameState.getWorld(), gameState.getGameObjects()), 0);
    BOOST_CHECK_EQUAL(Cordinate::isValid(20, 18, gameState.getWorld(), gameState.getGameObjects()), 0);
    // Y en dehors de la map, return false
    BOOST_CHECK_EQUAL(Cordinate::isValid(8, -1, gameState.getWorld(), gameState.getGameObjects()), 0);
    BOOST_CHECK_EQUAL(Cordinate::isValid(8, 20, gameState.getWorld(), gameState.getGameObjects()), 0);
    // Destination valide, return true
    BOOST_CHECK_EQUAL(Cordinate::isValid(3, 6, gameState.getWorld(), gameState.getGameObjects()), 1);
    // Destination obstruée par une unité, return false
    BOOST_CHECK_EQUAL(Cordinate::isValid(1, 3, gameState.getWorld(), gameState.getGameObjects()), 0);
    // Destination obstruée par une spawn Area, return false
    BOOST_CHECK_EQUAL(Cordinate::isValid(0, 0, gameState.getWorld(), gameState.getGameObjects()), 0);
}

BOOST_AUTO_TEST_CASE(TestCordinateFuncIsDestination)
{
    // Cordonnées de destination différentes des coordonnées actuelles, return false
    Node destination = Node{.x = 1, .y = 3};
    BOOST_CHECK_EQUAL(Cordinate::isDestination(1, 8, destination), 0);
    // Cordonnées de destination égales aux coordonnées actuelles, return true
    BOOST_CHECK_EQUAL(Cordinate::isDestination(1, 3, destination), 1);
}

BOOST_AUTO_TEST_CASE(TestCordinateFuncCalculateH)
{
    // Vérifie le calcul euclidien entre deux cases
    Node destination = Node{.x = 1, .y = 3};
    BOOST_CHECK_EQUAL(Cordinate::calculateH(1, 8, destination), 5);
    BOOST_CHECK_EQUAL(Cordinate::calculateH(1, 5, destination), 2);
    BOOST_CHECK_EQUAL(Cordinate::calculateH(-1, 8, destination), 7);
    BOOST_CHECK_EQUAL(Cordinate::calculateH(1, -5, destination), 8);
}

BOOST_AUTO_TEST_CASE(TestCordinateFuncAstarNoObstacle)
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
        // 4. Chercher une unité présente dans le player active qui a pour coordonnée le x et y choisi.
        int x = 2;
        int y = 2;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->setSelectedUnit(make_shared<Character>(unit));
            }
        }
        Node depart = Node{.x = engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()};
        Node destination = Node{.x = 0, .y = 4};
        vector<Node> nodes = Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), engine->getGameState().getGameObjects(), engine->getSelectedUnit().get()->getPm());
        BOOST_CHECK_EQUAL(nodes.size(), 5);
    }
}

BOOST_AUTO_TEST_CASE(TestCordinateFuncAstarDestinationNotFound)
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
        // 4. Chercher une unité présente dans le player active qui a pour coordonnée le x et y choisi.
        int x = 2;
        int y = 2;
        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
            if(unit.getX() == x && unit.getY() == y){
                engine->setSelectedUnit(make_shared<Character>(unit));
            }
        }
        Node depart = Node{.x = engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()};
        Node destination = Node{.x = 8, .y = 0};
        vector<Node> nodes = Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), engine->getGameState().getGameObjects(), engine->getSelectedUnit().get()->getPm());
        BOOST_CHECK_EQUAL(nodes.size(), 1);
    }
}

/* vim: set sw=2 sts=2 et : */
