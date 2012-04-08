#include "data/Event.h"
#include "test-lib-data-events-2_64_8.h"

CPPUNIT_TEST_SUITE_REGISTRATION( Event_2_64_8_Test );

unsigned int Event_2_64_8_Test::getDataSize() const {
  return 150;
}

void Event_2_64_8_Test::setUp() {
  dataFormat_ = bo::data::Format(1, 2, 64);
}

