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

BOOST_AUTO_TEST_CASE(TestEngine)
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
        BOOST_CHECK_EQUAL(engine->getGameState().getAttackMode(), 0);
        engine->getGameState().setAttackMode(true);
        BOOST_CHECK_EQUAL(engine->getGameState().getAttackMode(), 1);
        engine->getGameState().unselectedUnit();
        BOOST_CHECK_EQUAL(engine->getGameState().getSelectedUnit(), nullptr);
        BOOST_CHECK_THROW( engine->addCommand(nullptr, 1), std::exception );
        shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
        BOOST_REQUIRE_NO_THROW( engine->addCommand(newTurnCommand, 1));
    }

}
