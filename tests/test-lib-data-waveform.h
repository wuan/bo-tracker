#ifndef TESTS_TEST_LIB_DATA_WAVEFORM_H_
#define TESTS_TEST_LIB_DATA_WAVEFORM_H_

#include <boost/shared_ptr.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>  
 
#include "namespaces.h"
#include "data/Waveform.h"
 
class WaveformTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( WaveformTest );
  CPPUNIT_TEST( testCreate );
  CPPUNIT_TEST_SUITE_END();

  public:
  void setUp();
  void tearDown();

  //! tests

  void testCreate();
};

#endif
