#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestWeaponGettersSettersConstrutor)
{
    { // GET & SET pm
       state::Weapon weapon;
       BOOST_CHECK_EQUAL(weapon.getPm(), 0);
       weapon.setPm(5);
       BOOST_CHECK_EQUAL(weapon.getPm(), 5);
    }

    { // GET & SET damage
        state::Weapon weapon;
        BOOST_CHECK_EQUAL(weapon.getDamage(), 0);
        weapon.setDamage(5);
        BOOST_CHECK_EQUAL(weapon.getDamage(), 5);
    }

    { // GET & SET rangeMin & rangeMax
        state::Weapon weapon;
        BOOST_CHECK_EQUAL(weapon.getRangeMin(), 0);
        BOOST_CHECK_EQUAL(weapon.getRangeMax(), 0);
        weapon.setRangeMax(5);
        BOOST_CHECK_EQUAL(weapon.getRangeMax(), 5);
    }

    { // GET & SET damageAreaMax
        state::Weapon weapon;
        BOOST_CHECK_EQUAL(weapon.getDamageAreaMax(), 0);
        weapon.setDamageAreaMax(5);
        BOOST_CHECK_EQUAL(weapon.getDamageAreaMax(), 5);
    }

    { // Constructor
        state::Weapon weapon{3,5, 50, 0, 5, 5, state::DirectionType::CROSS};
        BOOST_CHECK_EQUAL(weapon.getLevel(),3);
        BOOST_CHECK_EQUAL(weapon.getPm(), 5);
        BOOST_CHECK_EQUAL(weapon.getDamage(), 50);
        BOOST_CHECK_EQUAL(weapon.getRangeMin(), 0);
        BOOST_CHECK_EQUAL(weapon.getRangeMax(), 5);
        BOOST_CHECK_EQUAL(weapon.getDamageAreaMax(), 5);
        BOOST_CHECK_EQUAL(weapon.getDirection(), state::DirectionType::CROSS);
    }


  /*
    BOOST_CHECK_LE(ex.x, 32); // Less than equal
    BOOST_CHECK_GT(ex.x, 11); // Greater than equl
  */
}

/* vim: set sw=2 sts=2 et : */
