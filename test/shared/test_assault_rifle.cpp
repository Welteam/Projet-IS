#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestSniperGettersSettersConstrutors)
{
    { // Constructor whith parameters
        state::AssaultRifle assaultRifle;
        BOOST_CHECK_EQUAL(assaultRifle.getPm(), 5);
        BOOST_CHECK_EQUAL(assaultRifle.getDamage(), 10);
        BOOST_CHECK_EQUAL(assaultRifle.getRangeMin(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestSniperGettersSettersConstrutors2)
{
    { // Constructor with parameters
        state::AssaultRifle assaultRifle;
        BOOST_CHECK_EQUAL(assaultRifle.getRangeMax(), 5);
        BOOST_CHECK_EQUAL(assaultRifle.getDamageAreaMax(), 0);
        BOOST_CHECK_EQUAL(assaultRifle.getDirection(), state::DirectionType::FULL);
    }
}

/* vim: set sw=2 sts=2 et : */
