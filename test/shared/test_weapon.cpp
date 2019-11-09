#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestWeaponGettersSettersConstrutor)
{/*
    { // GET & SET pm
       state::Weapon weapon;
       BOOST_CHECK_EQUAL(weapon.getLpMax(), 100);
       BOOST_CHECK_EQUAL(weapon.getPm(), 5);
       weapon.setPm(6);
       BOOST_CHECK_EQUAL(weapon.getPm(), 6);
    }

    { // GET & SET damage
        state::Weapon weapon;
        BOOST_CHECK_EQUAL(weapon.getDamage(), 30);
        weapon.setDamage(40);
        BOOST_CHECK_EQUAL(weapon.getDamage(), 40);
    }

    { // GET & SET rangeMin & rangeMax
        state::Weapon weapon;
        BOOST_CHECK_EQUAL(weapon.getRangeMin(), 0);
        BOOST_CHECK_EQUAL(weapon.getRangeMax(), 5);
        weapon.setRangeMax(6);
        BOOST_CHECK_EQUAL(weapon.getRangeMax(), 6);
    }

    { // GET & SET damageAreaMax
        state::Weapon weapon;
        BOOST_CHECK_EQUAL(weapon.getDamageAreaMax(), 0);
        weapon.setDamageAreaMax(5);
        BOOST_CHECK_EQUAL(weapon.getDamageAreaMax(), 5);
    }

    {
        state::Weapon weapon;
        weapon.levelUp();
        BOOST_CHECK_BITWISE_EQUAL(weapon.getLevel(), 1);
        weapon.levelUp();
        BOOST_CHECK_BITWISE_EQUAL(weapon.getLevel(), 2);
        weapon.levelUp();
        BOOST_CHECK_BITWISE_EQUAL(weapon.getLevel(), 3);
    }

    { // Constructor
        state::Weapon weapon{3,160, 5, 50, 0, 5, 5, state::DirectionType::CROSS};
        BOOST_CHECK_EQUAL(weapon.getLevel(),3);
        BOOST_CHECK_EQUAL(weapon.getLpMax(),160);
        BOOST_CHECK_EQUAL(weapon.getPm(), 5);
        BOOST_CHECK_EQUAL(weapon.getDamage(), 50);
        BOOST_CHECK_EQUAL(weapon.getRangeMin(), 0);
        BOOST_CHECK_EQUAL(weapon.getRangeMax(), 5);
        BOOST_CHECK_EQUAL(weapon.getDamageAreaMax(), 5);
        BOOST_CHECK_EQUAL(weapon.getDirection(), state::DirectionType::CROSS);
    }*/


  /*
    BOOST_CHECK_LE(ex.x, 32); // Less than equal
    BOOST_CHECK_GT(ex.x, 11); // Greater than equl
  */
}

/* vim: set sw=2 sts=2 et : */
