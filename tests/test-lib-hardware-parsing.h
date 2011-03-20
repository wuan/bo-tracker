#ifndef TESTS_TEST_LIB_HARDWARE_PARSING_H_
#define TESTS_TEST_LIB_HARDWARE_PARSING_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
 
class HardwareParsingTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( HardwareParsingTest );
  CPPUNIT_TEST( testTicksParsing );
  CPPUNIT_TEST( testSamplesParsing );
  CPPUNIT_TEST_SUITE_END();

  private:

  public:
  void setUp();
  void tearDown();
  void testTicksParsingTest(const std::string& input, const std::string& dateTime, unsigned int counterValue,
      float longitude, float latitude, short altitude,
      unsigned short numberOfSatellites, const std::string& gpsStatus);

  //! tests

  void testTicksParsing();
  void testTicksParsingF25();
  void testSamplesParsing();
};

#endif
