#ifndef TESTS_TEST_LIB_HARDWARE_PARSING_H_
#define TESTS_TEST_LIB_HARDWARE_PARSING_H_

#include <boost/shared_ptr.hpp>
 
#include "namespaces.h"
 
class HardwareParsingTest : public CPPUNIT_NS :: TestFixture
{
  CPPUNIT_TEST_SUITE( HardwareParsingTest );
  CPPUNIT_TEST( testTicksParsing );
  CPPUNIT_TEST( testTicksParsingF25 );
  CPPUNIT_TEST( testSamplesParsing );
  CPPUNIT_TEST_SUITE_END();

  private:

  public:
  void setUp();
  void tearDown();
  void ticksParsingTest(const std::string& input, const pt::ptime& dateTime, unsigned int counterValue,
      float longitude, float latitude, short altitude,
      unsigned short numberOfSatellites, const std::string& gpsStatus, const std::string& firmwareVersion);

  //! tests

  void testTicksParsing();
  void testTicksParsingF25();
  void testSamplesParsing();
};

#endif
