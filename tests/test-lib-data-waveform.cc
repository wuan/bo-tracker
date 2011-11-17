#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/Event.h"

#include "test-lib-data-waveform.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WaveformTest );

void WaveformTest::setUp() {
}

void WaveformTest::tearDown() {
}

void WaveformTest::testCreate() {
  bo::data::Format format(8,2,64);

  bo::data::Array::AP array(new bo::data::Array(format));

  pt::ptime now;
  bo::data::Waveform wfm(array, now, pt::nanoseconds(3125));
}

