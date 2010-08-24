#ifndef TESTS_TEST_LIB_DATA_SAMPLES_H_
#define TESTS_TEST_LIB_DATA_SAMPLES_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
#include "data/Samples.h"
 
class SampleTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( SampleTest );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testWrite );
  CPPUNIT_TEST( testAppend );
  CPPUNIT_TEST( testSize );
  CPPUNIT_TEST_SUITE_END();

  private:
  bo::data::sample::Base::Creator::P sampleCreator_;

  bo::data::Samples::P getSamples1();
  bo::data::Samples::P getSamples2();
  bo::data::sample::Base::AP getSample();
  bo::data::sample::Base::AP getSample(const pt::ptime&);

  public:
  void setUp();
  void tearDown();

  //! tests

  void testAdd();

  void testWrite();

  void testAppend();

  void testSize();
};

#endif
