#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "test-lib-data-arrayof.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ArrayOfTest );

void ArrayOfTest::setUp() {
}

void ArrayOfTest::tearDown() {
}

void ArrayOfTest::testCreate() {

  bo::data::Array::AP array(new bo::data::ArrayOf<signed char>(2, 64));

  CPPUNIT_ASSERT_EQUAL((unsigned short)2, array->getNumberOfChannels());
  CPPUNIT_ASSERT_EQUAL((unsigned int)64, array->getNumberOfSamples());
}

void ArrayOfTest::testSetAndGet() {

  bo::data::Array::AP array(new bo::data::ArrayOf<signed char>(2, 64));

  for (unsigned short channel = 0; channel < array->getNumberOfChannels(); channel++) {
    for (unsigned int sample = 0; sample < array->getNumberOfSamples(); sample++) {
      array->setInt((signed int)channel + sample * array->getNumberOfChannels(), sample, channel);
    }
  }

  for (unsigned short channel = 0; channel < array->getNumberOfChannels(); channel++) {
    for (unsigned int sample = 0; sample < array->getNumberOfSamples(); sample++) {
      signed char expectedValue = channel + sample * array->getNumberOfChannels();
      int value = array->getInt(sample, channel);
      CPPUNIT_ASSERT_EQUAL((int)expectedValue, value);
    }
  }
}

