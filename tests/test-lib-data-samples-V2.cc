#include "data/Sample.h"
#include "data/sample/V2.h"
#include "test-lib-data-samples-V2.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SampleV2Test );

unsigned int SampleV2Test::getDataSize() const {
  return 278;
}

void SampleV2Test::setUp() {
  sampleCreator_ = bo::data::sample::Base::Creator::P(new bo::data::sample::V2::Creator());
}

