#include "data/Event.h"
#include "data/Format.h"
#include "test-lib-data-events-2_1_12.h"

CPPUNIT_TEST_SUITE_REGISTRATION( Event_2_1_12_Test );

unsigned int Event_2_1_12_Test::getDataSize() const {
  return 24;
}

void Event_2_1_12_Test::setUp() {
  dataFormat_ = bo::data::Format(2, 2, 1);
}

