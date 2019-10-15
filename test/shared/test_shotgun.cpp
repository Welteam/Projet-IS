#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestShotgunConstructorWithParameters)
{
    { // Constructor whith parameters
        state::Shotgun shotgun;
        BOOST_CHECK_EQUAL(shotgun.getPm(), 7);
        BOOST_CHECK_EQUAL(shotgun.getDamage(), 60);
        BOOST_CHECK_EQUAL(shotgun.getRangeMin(), 1);
        //BOOST_CHECK_EQUAL(shotgun.getRangeMax(), 5);
        //BOOST_CHECK_EQUAL(shotgun.getDamageAreaMax(), 2);
        //BOOST_CHECK_EQUAL(shotgun.getDirection(), 2);
    }
}

BOOST_AUTO_TEST_CASE(TestShotgunConstructorWithParameters2)
{
    { // Constructor whith parameters
        state::Shotgun shotgun;
        //BOOST_CHECK_EQUAL(shotgun.getPm(), 7);
        //BOOST_CHECK_EQUAL(shotgun.getDamage(), 60);
        //BOOST_CHECK_EQUAL(shotgun.getRangeMin(), 1);
        BOOST_CHECK_EQUAL(shotgun.getRangeMax(), 5);
        BOOST_CHECK_EQUAL(shotgun.getDamageAreaMax(), 2);
        BOOST_CHECK_EQUAL(shotgun.getDirection(), 2);
    }
}

/* vim: set sw=2 sts=2 et : */
