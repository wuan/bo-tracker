#include "data/Event.h"
#include "data/Format.h"
#include "test-lib-data-events-V1.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EventV1Test );

unsigned int EventV1Test::getDataSize() const {
  return 22;
}

void EventV1Test::setUp() {
  dataFormat_ = bo::data::Format::AP(new bo::data::Format(8, 2, 1));
}

