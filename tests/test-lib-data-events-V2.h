#ifndef TESTS_TEST_LIB_DATA_EVENTS_V2_H_
#define TESTS_TEST_LIB_DATA_EVENTS_V2_H_

#include "namespaces.h"
#include "test-lib-data-events.h"
 
class EventV2Test : public EventTest
{
  CPPUNIT_TEST_SUITE( EventV2Test );
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
