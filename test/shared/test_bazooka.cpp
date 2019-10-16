#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestSniperGettersSettersConstrutors)
{
    { // Constructor whith parameters
        state::Bazooka bazooka;
        BOOST_CHECK_EQUAL(bazooka.getPm(), 5);
        BOOST_CHECK_EQUAL(bazooka.getDamage(), 10);
        BOOST_CHECK_EQUAL(bazooka.getRangeMin(), 1);
    }
}

BOOST_AUTO_TEST_CASE(TestSniperGettersSettersConstrutors2)
{
    { // Constructor with parameters
        state::Bazooka bazooka;
        BOOST_CHECK_EQUAL(bazooka.getRangeMax(), 5);
        BOOST_CHECK_EQUAL(bazooka.getDamageAreaMax(), 0);
        BOOST_CHECK_EQUAL(bazooka.getDirection(), state::DirectionType::FULL);
    }
}

/* vim: set sw=2 sts=2 et : */
