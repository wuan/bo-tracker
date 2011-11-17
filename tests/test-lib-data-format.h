#ifndef TESTS_TEST_LIB_DATA_FORMAT_H_
#define TESTS_TEST_LIB_DATA_FORMAT_H_

#include <boost/shared_ptr.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
  
#include "namespaces.h"
#include "data/Format.h"
 
class FormatTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( FormatTest );
  CPPUNIT_TEST( testIndex );
  CPPUNIT_TEST( testOffset );
  CPPUNIT_TEST_SUITE_END();

  public:
  void setUp();
  void tearDown();

  void doIndexTest(const bo::data::Format&& format);
  void doOffsetTest(const bo::data::Format&& format);

  //! tests

  void testIndex();
  void testOffset();
};

#endif
