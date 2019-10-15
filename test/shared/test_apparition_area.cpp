
#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameObject)
{
  {
    state::ApparitionArea apparitionArea {};
    BOOST_CHECK_EQUAL(apparitionArea.getX(), 0);
    BOOST_CHECK_EQUAL(apparitionArea.getY(), 0);
  }

  {
    state::ApparitionArea apparitionArea {5, 10};
    BOOST_CHECK_EQUAL(apparitionArea.getX(), 5);
    BOOST_CHECK_EQUAL(apparitionArea.getY(), 10);
  }

}

/* vim: set sw=2 sts=2 et : */
