#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestSniperGettersSettersConstrutors)
{
    { // Constructor whith parameters
        state::Sniper sniper;
        BOOST_CHECK_EQUAL(sniper.getPm(), 2);
        BOOST_CHECK_EQUAL(sniper.getDamage(), 70);
        BOOST_CHECK_EQUAL(sniper.getRangeMin(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestSniperGettersSettersConstrutors2)
{
    { // Constructor whith parameters
        state::Sniper sniper;
        BOOST_CHECK_EQUAL(sniper.getRangeMax(), 50);
        BOOST_CHECK_EQUAL(sniper.getDamageAreaMax(), 0);
        BOOST_CHECK_EQUAL(sniper.getDirection(), state::DirectionType::FULL);
    }
}

/* vim: set sw=2 sts=2 et : */
