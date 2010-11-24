#ifndef TESTS_TEST_LIB_DATA_SAMPLES_V2_H_
#define TESTS_TEST_LIB_DATA_SAMPLES_V2_H_

#include "namespaces.h"
#include "test-lib-data-samples.h"
 
class SampleV2Test : public SampleTest
{
  CPPUNIT_TEST_SUITE( SampleV2Test );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testWrite );
  CPPUNIT_TEST( testAppend );
  CPPUNIT_TEST( testSize );
  CPPUNIT_TEST_SUITE_END();

  protected:
  virtual unsigned int getDataSize() const;

  public:
  void setUp();
};

#endif
