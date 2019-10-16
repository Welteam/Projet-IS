
#include <boost/test/unit_test.hpp>
#include <string>
#include "../../src/shared/state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameObject)
{
  {
    state::GameState gameState{};
    BOOST_CHECK_EQUAL(gameState.getTurn(), 0);
    gameState.setTurn(1);
    BOOST_CHECK_EQUAL(gameState.getTurn(), 1);
  }

  {
      state::GameState gameState{};
      BOOST_CHECK_EQUAL(gameState.getWorld().getXStep(), 16);
      std::string filename = "test.txt";
      state::World tileMap = state::World(filename);
      gameState.setWorld(tileMap);
      BOOST_CHECK_EQUAL(gameState.getWorld().getXStep(), 32);
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


}

/* vim: set sw=2 sts=2 et : */
