#ifndef TESTS_TEST_LIB_DATA_ARRAYOF_H_
#define TESTS_TEST_LIB_DATA_ARRAYOF_H_

#include <boost/shared_ptr.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>  
 
#include "namespaces.h"
#include "data/ArrayOf.h"
 
class ArrayOfTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( ArrayOfTest );
  CPPUNIT_TEST( testCreate );
  CPPUNIT_TEST( testSetAndGet );
  CPPUNIT_TEST_SUITE_END();

  public:
  void setUp();
  void tearDown();

  //! tests

  void testCreate();

  void testSetAndGet();
};

#endif
