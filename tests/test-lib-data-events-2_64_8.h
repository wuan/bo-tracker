#ifndef TESTS_TEST_LIB_DATA_EVENTS_2_64_8_H_
#define TESTS_TEST_LIB_DATA_EVENTS_2_64_8_H_

#include "namespaces.h"
#include "test-lib-data-events.h"
 
class Event_2_64_8_Test : public EventTest
{
  CPPUNIT_TEST_SUITE( Event_2_64_8_Test );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testWrite );
  CPPUNIT_TEST( testAppend );
  CPPUNIT_TEST( testMultipleAppend );
  CPPUNIT_TEST( testSize );
  CPPUNIT_TEST_SUITE_END();

  protected:
  virtual unsigned int getDataSize() const;

  public:
  void setUp();
};

#endif
