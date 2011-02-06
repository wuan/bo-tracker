#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cmath>

#include "hardware/parsing/Ticks.h"
#include "util/String.h"


#include "test-lib-hardware-parsing.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HardwareParsingTest );

void HardwareParsingTest::setUp() {
}

void HardwareParsingTest::tearDown() {
}

void HardwareParsingTest::testTicksParsingTest(const std::string& input, const std::string& dateTime, unsigned int counterValue, float longitude, float latitude, float altitude, unsigned short numberOfSatellites, const std::string& gpsStatus) {
  std::vector<std::string> fields;

  bo::util::String::split(input, fields, ",");

  bo::hardware::parsing::Ticks ticksParser(fields);

  CPPUNIT_ASSERT_EQUAL(dateTime, ticksParser.getDateTime());
  CPPUNIT_ASSERT_EQUAL(counterValue, ticksParser.getCounterValue());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(longitude, ticksParser.getLongitude(), 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(latitude, ticksParser.getLatitude(), 1e-4);
  if (! isnan(altitude))
    CPPUNIT_ASSERT_DOUBLES_EQUAL(altitude, ticksParser.getAltitude(), 1e-4);
  else
    CPPUNIT_ASSERT(isnan(ticksParser.getAltitude()));
  CPPUNIT_ASSERT_EQUAL(numberOfSatellites, ticksParser.getNumberOfSatellites());
  CPPUNIT_ASSERT_EQUAL(gpsStatus[0], ticksParser.getGpsStatus());
}

void HardwareParsingTest::testTicksParsing() {
  testTicksParsingTest("BLSEC,185630,190810,A,4729.2852,N,01904.2683,E,AB852D",
      "190810 185630",
      11240749,
      19.0711, 47.4881, NAN,
      0, "A");
  testTicksParsingTest("BLSEC,185631,190810,V,4729.2856,S,01904.2688,W,D1ABEA",
      "190810 185631",
      13741034,
      -19.0711, -47.4881, NAN,
      0, "V");

  testTicksParsingTest("BLSEC,0833CC,A,131531,200810,4808.1189,N,01132.6299,E,576.5000,M,06",
      "200810 131531",
      537548,
      11.5438, 48.1353, 576.5,
      6, "A");
  testTicksParsingTest("BLSEC,2E59FE,V,131532,200810,4808.1187,N,01132.6301,E,577.2000,M,06",
      "200810 131532",
      3037694,
      11.5438, 48.1353, 577.2,
      6, "V");
}

void HardwareParsingTest::testSamplesParsing() {
  //"BLSIG,F133E6,CF4,860");
  //"BLSIG,F1341D,450,77C");

  //"BLSEQ,F67E15,667E667D6A7E707F787F817F887F8C808F80907F8F809381968191808A7F847F7F7F737E647D587D507D487B467B4D7C587C647C747D887F9780A181A983AF84B084AD84AA83A782A0819A80957F907F8A7D847E807E7C7D777B747C727C727B717B717D727F737F75807682778279827A817A817B807C7E7C7C7A7B797B777A"
  //"BLSEQ,00D13B,4C7E1A8103831E834F7F7F7DB07EDE7DF37AF17BEC7FE580D17DB77EA3828D81747E6A7F6D81697F627C607E61805F7E617E69816F827280787F79807780767E7B7D797D727D6F7D747E757E727F73807D80837E847D867E877E827D7D7D7B7F777E737C737C767D767C7A7D817E8580858089818B818B828D819180917E927D"
}

