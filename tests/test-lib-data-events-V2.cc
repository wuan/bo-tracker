#include "data/Event.h"
#include "test-lib-data-events-V2.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EventV2Test );

unsigned int EventV2Test::getDataSize() const {
  return 278;
}

void EventV2Test::setUp() {
  dataFormat_ = bo::data::Format::AP(new bo::data::Format(8, 2, 64));
}

