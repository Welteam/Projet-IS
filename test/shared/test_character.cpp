
#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameObject)
{
  {
    state::Character character {};
    BOOST_CHECK_EQUAL(character.getX(), 0);
    BOOST_CHECK_EQUAL(character.getY(), 0);
  }

  {
    state::Character character {};
    BOOST_CHECK_EQUAL(character.getLp(), 100);
    character.setLp(50);
    BOOST_CHECK_EQUAL(character.getLp(), 50);
  }

  {
    state::Character character {};
    state::Weapon weapon {};
    BOOST_CHECK_EQUAL(character.getWeapon().getLevel(), 0);
    weapon.levelUp();
    character.setWeapon(weapon);
    BOOST_CHECK_EQUAL(character.getWeapon().getLevel(), 1);
  }

  {
    state::Character character {};
    character.setLp(110);
    character.levelUp();
    BOOST_CHECK_EQUAL(character.getWeapon().getLevel(), 1);
    BOOST_CHECK_EQUAL(character.getLp(), 120);
  }

  {
    state::Character character {5, 5, 75};
    BOOST_CHECK_EQUAL(character.getX(), 5);
    BOOST_CHECK_EQUAL(character.getY(), 5);
    BOOST_CHECK_EQUAL(character.getLp(), 75);
  }

}

/* vim: set sw=2 sts=2 et : */
