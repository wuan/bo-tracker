#include "data/Format.h"

#include "test-lib-data-format.h"

CPPUNIT_TEST_SUITE_REGISTRATION( FormatTest );

void FormatTest::setUp() {
}

void FormatTest::tearDown() {
}

void FormatTest::testIndex() {
  bo::data::Format::CP format(new bo::data::Format(8,2,64));

  CPPUNIT_ASSERT_EQUAL(1, format->getNumberOfBytesPerSample());
}

