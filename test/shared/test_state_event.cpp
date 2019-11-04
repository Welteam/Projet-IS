
#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameObject)
{
  {
    state::StateEvent stateEvent {state::StateEventID::WORLD};
    BOOST_CHECK_EQUAL(stateEvent.getStateEventID(), state::StateEventID::WORLD);
    stateEvent.setStateEventID(state::StateEventID::PLAYER1);
    BOOST_CHECK_EQUAL(stateEvent.getStateEventID(), state::StateEventID::PLAYER1);
  }

  {
    state::Character character {};
    BOOST_CHECK_EQUAL(character.getHp(), 100);
    character.setHp(50);
    BOOST_CHECK_EQUAL(character.getHp(), 50);
  }

}

/* vim: set sw=2 sts=2 et : */
