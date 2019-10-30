
#include <boost/test/unit_test.hpp>
#include <string>
#include "../../src/shared/state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameObject)
{
    std::string mapWorld = "../../../res/map.txt";
  {
    state::GameState gameState{};
    BOOST_CHECK_EQUAL(gameState.getTurn(), 0);
    gameState.setTurn(1);
    BOOST_CHECK_EQUAL(gameState.getTurn(), 1);
  }

  {
      state::GameState gameState{};
      BOOST_CHECK_EQUAL(gameState.getWorld().getXMax(), 0);
      BOOST_CHECK_EQUAL(gameState.getWorld().getYMax(), 0);
      state::World tileMap = state::World(mapWorld);
      gameState.setWorld(tileMap);
      BOOST_CHECK_EQUAL(gameState.getWorld().getXMax(), 20);
      BOOST_CHECK_EQUAL(gameState.getWorld().getYMax(), 20);
  }

  {
      state::GameState gameState{};
      BOOST_CHECK_EQUAL(gameState.getPlayer1().getId(), 0);
      BOOST_CHECK_EQUAL(gameState.getPlayer2().getId(), 0);
      state::Player player1 = state::Player(1, std::vector<state::Character>(), std::vector<state::Tower>(), std::vector<state::ApparitionArea>());
      state::Player player2 = state::Player(2, std::vector<state::Character>(), std::vector<state::Tower>(), std::vector<state::ApparitionArea>());
      gameState.setPlayer1(player1);
      gameState.setPlayer2(player2);
      BOOST_CHECK_EQUAL(gameState.getPlayer1().getId(), 1);
      BOOST_CHECK_EQUAL(gameState.getPlayer2().getId(), 2);
  }

  {
      state::GameState gameState{mapWorld};
      BOOST_CHECK_EQUAL(gameState.getTurn(), 0);
      BOOST_CHECK_EQUAL(gameState.getWorld().getXMax(), 20);
      BOOST_CHECK_EQUAL(gameState.getWorld().getYMax(), 20);
      BOOST_CHECK_EQUAL(gameState.getPlayer1().getId(), 0);
      BOOST_CHECK_EQUAL(gameState.getPlayer2().getId(), 0);
  }

}

/* vim: set sw=2 sts=2 et : */
