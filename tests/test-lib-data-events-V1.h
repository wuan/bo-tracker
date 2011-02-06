#ifndef TESTS_TEST_LIB_DATA_SAMPLES_V1_H_
#define TESTS_TEST_LIB_DATA_SAMPLES_V1_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
#include "data/Events.h"
#include "test-lib-data-events.h"
 
class EventV1Test : public EventTest
{
  CPPUNIT_TEST_SUITE( EventV1Test );
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
