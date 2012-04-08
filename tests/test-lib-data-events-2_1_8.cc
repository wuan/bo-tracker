#include "data/Event.h"
#include "data/Format.h"
#include "test-lib-data-events-2_1_8.h"

CPPUNIT_TEST_SUITE_REGISTRATION( Event_2_1_8_Test );

unsigned int Event_2_1_8_Test::getDataSize() const {
  return 22;
}

void Event_2_1_8_Test::setUp() {
  dataFormat_ = bo::data::Format(1, 2, 1);
}

