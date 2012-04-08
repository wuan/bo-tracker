#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/Format.h"
#include "data/Event.h"

#include "test-lib-data-waveform.h"

CPPUNIT_TEST_SUITE_REGISTRATION( WaveformTest );

void WaveformTest::setUp() {
}

void WaveformTest::tearDown() {
}

void WaveformTest::testCreate() {
  bo::data::Format format(1, 2, 64);

  pt::ptime now;
  bo::data::Waveform::AP wfm(format.createWaveform(now, pt::nanoseconds(3125)));

  CPPUNIT_ASSERT_EQUAL((unsigned short)2, wfm->getNumberOfChannels());
  CPPUNIT_ASSERT_EQUAL((unsigned int)64, wfm->getNumberOfSamples());
}

