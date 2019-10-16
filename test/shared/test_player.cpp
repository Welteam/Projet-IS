#include "../../src/shared/state.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPlayerGettersEmptyConstrutors)
{
    state::Player player;
    BOOST_CHECK_EQUAL(player.getId(), 0);
    BOOST_CHECK_EQUAL(player.getUnits().empty(), 1);
    BOOST_CHECK_EQUAL(player.getTowers().empty(), 1);
    BOOST_CHECK_EQUAL(player.getApparitionAreas().empty(), 1);
}

BOOST_AUTO_TEST_CASE(TestPlayerSettersEmptyConstrutors)
{
    state::Player player;

    state::Character character1, character2, character3;
    state::Tower tower1, tower2;
    state::ApparitionArea apparitionArea1;

    std::vector<state::Character> units;
    units.push_back(character1);
    units.push_back(character2);
    units.push_back(character3);
    std::vector<state::Tower> towers;
    towers.push_back(tower1);
    towers.push_back(tower2);
    std::vector<state::ApparitionArea> apparitionAreas;
    apparitionAreas.push_back(apparitionArea1);

    player.setUnits(units);
    player.setTowers(towers);
    player.setApparitionAreas(apparitionAreas);

    BOOST_CHECK_EQUAL(player.getUnits().size(), 3);
    BOOST_CHECK_EQUAL(player.getTowers().size(), 2);
    BOOST_CHECK_EQUAL(player.getApparitionAreas().size(), 1);
}


BOOST_AUTO_TEST_CASE(TestPlayerConstrutorsWithParameters)
{
    state::Character character1, character2, character3;
    state::Tower tower1, tower2;
    state::ApparitionArea apparitionArea1;

    std::vector<state::Character> units;
    units.push_back(character1);
    units.push_back(character2);
    units.push_back(character3);
    std::vector<state::Tower> towers;
    towers.push_back(tower1);
    towers.push_back(tower2);
    std::vector<state::ApparitionArea> apparitionAreas;
    apparitionAreas.push_back(apparitionArea1);

    state::Player player{0, units, towers, apparitionAreas};

    BOOST_CHECK_EQUAL(player.getUnits().size(), 3);
    BOOST_CHECK_EQUAL(player.getTowers().size(), 2);
    BOOST_CHECK_EQUAL(player.getApparitionAreas().size(), 1);
}

