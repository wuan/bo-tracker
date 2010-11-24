#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/Sample.h"

#include "test-lib-data-waveform.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WaveformTest );

void WaveformTest::setUp() {
}

void WaveformTest::tearDown() {
}

void WaveformTest::testCreate() {
  pt::ptime now;
  bo::data::Sample::Waveform wfm(now, pt::nanoseconds(3125));

}

