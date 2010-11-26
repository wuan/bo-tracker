#include "data/Sample.h"
#include "data/sample/V1.h"
#include "test-lib-data-samples-V1.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SampleV1Test );

unsigned int SampleV1Test::getDataSize() const {
  return 22;
}

void SampleV1Test::setUp() {
  sampleFactory_ = bo::data::SampleFactory::AP(new bo::data::sample::V1Factory());
}

