#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/shared/engine/DisplayAttack.cpp"

using namespace engine;
using namespace std;
using namespace state;

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestAttackCommandConstructor)
{
    {
        // 1. Intancier GameState
        GameState gameState{};

        // 2. Charger la carte World dans GameState
        gameState.setWorld(World{"../../../res/map.txt"});
        shared_ptr<Character> unit = make_shared<Character>(3, 8, 100, 5);
        vector<int> field = DisplayAttack::createField(unit.get(), gameState.getWorld(), gameState.getGameObjects());
        BOOST_CHECK_EQUAL(field.size(), 400);
        unit.get()->levelUp();
        vector<int> field1 = DisplayAttack::createField(unit.get(), gameState.getWorld(), gameState.getGameObjects());
        BOOST_CHECK_EQUAL(field1.size(), 400);
        unit.get()->levelUp();
        vector<int> field2 = DisplayAttack::createField(unit.get(), gameState.getWorld(), gameState.getGameObjects());
        BOOST_CHECK_EQUAL(field2.size(), 400);
        unit.get()->levelUp();
        vector<int> field3 = DisplayAttack::createField(unit.get(), gameState.getWorld(), gameState.getGameObjects());
        BOOST_CHECK_EQUAL(field3.size(), 400);
        unit.get()->levelUp();
        vector<int> field4 = DisplayAttack::createField(unit.get(), gameState.getWorld(), gameState.getGameObjects());
        BOOST_CHECK_EQUAL(field4.size(), 400);
    }
}