#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestTowerGettersSettersConstrutors)
{
    { // GET & SET lp, Empty constructor
       state::Tower tower;
       BOOST_CHECK_EQUAL(tower.getLp(), 100);
       tower.setLp(350);
       BOOST_CHECK_EQUAL(tower.getLp(), 350);
    }

    { // GET & SET coordinate x & y, Empty constructor
        state::Tower tower;
        BOOST_CHECK_EQUAL(tower.getX(), 0);
        BOOST_CHECK_EQUAL(tower.getY(), 0);
        tower.setX(5);
        tower.setY(15);
        BOOST_CHECK_EQUAL(tower.getX(), 5);
        BOOST_CHECK_EQUAL(tower.getY(), 15);
    }

    { // Constructor whith parameters
        state::Tower tower{200, 60, 120};
        BOOST_CHECK_EQUAL(tower.getLp(), 200);
        BOOST_CHECK_EQUAL(tower.getX(), 60);
        BOOST_CHECK_EQUAL(tower.getY(), 120);
    }
}

/* vim: set sw=2 sts=2 et : */
