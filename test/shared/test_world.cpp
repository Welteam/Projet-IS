#include "../../src/shared/state.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestWorldGettersEmptyConstrutors)
{
    state::World world;
    BOOST_CHECK_EQUAL(world.getXMax(), 0);
    BOOST_CHECK_EQUAL(world.getXStep(), 0);
    BOOST_CHECK_EQUAL(world.getYMax(), 0);
    BOOST_CHECK_EQUAL(world.getYStep(), 0);
    BOOST_CHECK_EQUAL(world.getTiles().empty(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnUnits1().empty(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnUnits2().empty(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnTowers1().empty(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnTowers2().empty(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnApparitionAreas1().empty(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnApparitionAreas1().empty(), 1);
}


BOOST_AUTO_TEST_CASE(TestWorldConctructorFile)
{
    /*BOOST_CHECK_EQUAL(world.getUnits().size(), 3);
    BOOST_CHECK_EQUAL(world.getTowers().size(), 2);
    BOOST_CHECK_EQUAL(world.getApparitionAreas().size(), 1);*/
}

/* vim: set sw=2 sts=2 et : */

