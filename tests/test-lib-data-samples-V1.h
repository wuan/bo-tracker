#ifndef TESTS_TEST_LIB_DATA_SAMPLES_V1_H_
#define TESTS_TEST_LIB_DATA_SAMPLES_V1_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
#include "data/Samples.h"
#include "test-lib-data-samples.h"
 
class SampleV1Test : public SampleTest
{
  CPPUNIT_TEST_SUITE( SampleV1Test );
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
