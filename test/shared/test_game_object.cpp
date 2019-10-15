
#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameObject)
{
  {
    state::GameObject gameObject {};
    BOOST_CHECK_EQUAL(gameObject.getX(), 0);
    BOOST_CHECK_EQUAL(gameObject.getY(), 0);
    gameObject.setX(21);
    BOOST_CHECK_EQUAL(gameObject.getX(), 21);
    gameObject.setY(21);
    BOOST_CHECK_EQUAL(gameObject.getY(), 21);
  }

  {
    state::GameObject gameObject {32, 11};
    BOOST_CHECK_EQUAL(gameObject.getX(), 32); // Less than equal
    BOOST_CHECK_EQUAL(gameObject.getY(), 11); // Greater than equl
  }


}

/* vim: set sw=2 sts=2 et : */
