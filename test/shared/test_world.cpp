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
    BOOST_CHECK_EQUAL(world.getXStep(), 16);
    BOOST_CHECK_EQUAL(world.getYMax(), 0);
    BOOST_CHECK_EQUAL(world.getYStep(), 16);
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
    std::string mapWorld = "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 4, 4, 4, 4, 4,"
                           "3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,"
                           "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,:"
                           "1, 2, 3, 7, 1, 17, 3, 12,/ 7, 10,/ 0, 0,:"
                           "1, 5, 3, 9, 4, 17, 1, 12,/ 9, 10,/ 0, 19,:";
    state::World world{mapWorld};
    BOOST_CHECK_EQUAL(world.getXMax(), 20);
    BOOST_CHECK_EQUAL(world.getXStep(), 16);
    BOOST_CHECK_EQUAL(world.getYMax(), 20);
    BOOST_CHECK_EQUAL(world.getYStep(), 16);
    BOOST_CHECK_EQUAL(world.getTiles().size(), 400);
    BOOST_CHECK_EQUAL(world.getSpawnUnits1().size(), 4);
    BOOST_CHECK_EQUAL(world.getSpawnUnits2().size(), 4);
    BOOST_CHECK_EQUAL(world.getSpawnTowers1().size(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnTowers2().size(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnApparitionAreas1().size(), 1);
    BOOST_CHECK_EQUAL(world.getSpawnApparitionAreas1().size(), 1);
}

/* vim: set sw=2 sts=2 et : */

