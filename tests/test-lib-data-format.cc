#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/Format.h"

#include "test-lib-data-format.h"

CPPUNIT_TEST_SUITE_REGISTRATION( FormatTest );

void FormatTest::setUp() {
}

void FormatTest::tearDown() {
}

void FormatTest::doIndexTest(bo::data::Format::CP format) {
  //std::cout << format->getNumberOfBytesPerSample() << std::endl;

  unsigned int lastIndex = -1;
  for (int sample = 0; sample < format->getNumberOfSamples(); sample++) {
    for (int channel = 0; channel < format->getNumberOfChannels(); channel++) {
      int index = format->getIndex(sample, channel);

      CPPUNIT_ASSERT_EQUAL(1u, index - lastIndex);

      lastIndex = index;
    }
  }
}

void FormatTest::testIndex() {

  doIndexTest(bo::data::Format::CP(new bo::data::Format(8,2,64)));
  doIndexTest(bo::data::Format::CP(new bo::data::Format(12,2,1)));

}

