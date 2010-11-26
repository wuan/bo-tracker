#ifndef TESTS_TEST_LIB_DATA_SAMPLES_H_
#define TESTS_TEST_LIB_DATA_SAMPLES_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
#include "data/Samples.h"
#include "data/SampleFactory.h"
 
class SampleTest : public CPPUNIT_NS :: TestFixture
{
//  CPPUNIT_TEST_SUITE( SampleTest );
//  CPPUNIT_TEST( testAdd );
//  CPPUNIT_TEST( testWrite );
//  CPPUNIT_TEST( testAppend );
//  CPPUNIT_TEST( testSize );
//  CPPUNIT_TEST_SUITE_END();

  private:

  bo::data::Samples::P getSamples1();
  bo::data::Samples::P getSamples2();
  bo::data::Sample::AP getSample(const pt::ptime&);

  protected:
  bo::data::SampleFactory::AP sampleFactory_;
  virtual unsigned int getDataSize() const = 0;

  public:
  virtual void setUp() = 0;
  virtual void tearDown();

  //! tests

  void testAdd();

  void testWrite();

  void testAppend();

  void testSize();
};

#endif

