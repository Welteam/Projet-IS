#include "../../src/shared/state.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

/*BOOST_AUTO_TEST_CASE(TestWorldGettersSetters)
{
    { // GET & SET tiles
        state::World world;
        BOOST_CHECK_EQUAL(world.getTiles().at(0), 0);
        world.setPm(5);
        BOOST_CHECK_EQUAL(weapon.getPm(), 5);
    }
}

BOOST_AUTO_TEST_CASE(TestWorlDrawPatternAlgorithme)
{
    {
        Exemple ex {};
        BOOST_CHECK_EQUAL(ex.x, 0);
        ex.setX(21);
        BOOST_CHECK_EQUAL(ex.x, 21);
    }
}

BOOST_AUTO_TEST_CASE(TestWorldConstructorReadTxtFile)
{
    {
        Exemple ex {};
        BOOST_CHECK_EQUAL(ex.x, 0);
        ex.setX(21);
        BOOST_CHECK_EQUAL(ex.x, 21);
    }
}*/

/* vim: set sw=2 sts=2 et : */

