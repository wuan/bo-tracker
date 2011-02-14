#ifndef TESTS_TEST_LIB_PROCESS_H_
#define TESTS_TEST_LIB_PROCESS_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
 
class ProcessTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( ProcessTest );
  CPPUNIT_TEST( testAroundMidnight );
  CPPUNIT_TEST_SUITE_END();

  public:
  void setUp();
  void tearDown();

  //! tests

  void testAroundMidnight();
};

#endif
