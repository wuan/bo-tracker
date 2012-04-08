#include "data/Event.h"
#include "test-lib-data-events-1_128_8.h"

CPPUNIT_TEST_SUITE_REGISTRATION( Event_1_128_8_Test );

unsigned int Event_1_128_8_Test::getDataSize() const {
  return 150;
}

void Event_1_128_8_Test::setUp() {
  dataFormat_ = bo::data::Format(1, 1, 128);
}

