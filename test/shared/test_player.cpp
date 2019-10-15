#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPlayerGettersSettersEmptyConstrutors)
{
    state::Player player;
    BOOST_CHECK_EQUAL(player.getId(), 0);
    BOOST_CHECK_EQUAL(player.getUnits().empty(), 1);
    BOOST_CHECK_EQUAL(player.getTowers().empty(), 1);
    BOOST_CHECK_EQUAL(player.getApparitionAreas().empty(), 1);
}

/*BOOST_AUTO_TEST_CASE(TestPlayerConstrutorsWithParameters)
{

    state::Player player;
    BOOST_CHECK_EQUAL(player.getId(), 0);
    BOOST_CHECK_EQUAL(player.getUnits().empty(), 1);
    BOOST_CHECK_EQUAL(player.getTowers().empty(), 1);
    BOOST_CHECK_EQUAL(player.getApparitionAreas().empty(), 1);
}*/

