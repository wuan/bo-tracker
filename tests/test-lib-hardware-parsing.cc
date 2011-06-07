#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cmath>

#include "hardware/comm/Dummy.h"
#include "hardware/gps/Dummy.h"
#include "hardware/parsing/Ticks.h"
#include "hardware/parsing/Samples.h"
#include "util/String.h"


#include "test-lib-hardware-parsing.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HardwareParsingTest );

class ParseBase : public bo::hardware::parsing::Base {

  public:
    unsigned int parseHexString(const std::string hexString) {
      return parseHex(hexString);
    }

};

void HardwareParsingTest::setUp() {
}

void HardwareParsingTest::tearDown() {
}

void HardwareParsingTest::ticksParsingTest(const std::string& input, const pt::ptime& dateTime, unsigned int counterValue, float longitude, float latitude, short altitude, unsigned short numberOfSatellites, const std::string& gpsStatus, const std::string& firmwareVersion) {
  std::vector<std::string> fields;

  bo::util::String::split(input, fields, ",");

  bo::hardware::parsing::Ticks ticksParser(fields);

  CPPUNIT_ASSERT_EQUAL(dateTime, ticksParser.getDateTime());
  CPPUNIT_ASSERT_EQUAL(counterValue, ticksParser.getCounterValue());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(longitude, ticksParser.getLongitude(), 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(latitude, ticksParser.getLatitude(), 1e-4);
  CPPUNIT_ASSERT_EQUAL(altitude, ticksParser.getAltitude());
  CPPUNIT_ASSERT_EQUAL(numberOfSatellites, ticksParser.getNumberOfSatellites());
  CPPUNIT_ASSERT_EQUAL(gpsStatus[0], ticksParser.getGpsStatus());
  CPPUNIT_ASSERT_EQUAL(firmwareVersion, ticksParser.getFirmwareVersion());
}

void HardwareParsingTest::testTicksParsing() {
  ticksParsingTest("BLSEC,185630,190810,A,4729.2852,N,01904.2683,E,AB852D",
      pt::ptime(gr::date(2010,8,19),pt::time_duration(18,56,30)),
      11240749,
      19.0711, 47.4881, 0,
      0, "A", "");
  ticksParsingTest("BLSEC,185631,190810,V,4729.2856,S,01904.2688,W,D1ABEA",
      pt::ptime(gr::date(2010,8,19),pt::time_duration(18,56,31)),
      13741034,
      -19.0711, -47.4881, 0,
      0, "V", "");

  ticksParsingTest("BLSEC,0833CC,A,131531,200810,4808.1189,N,01132.6299,E,576.5000,M,06",
      pt::ptime(gr::date(2010,8,20),pt::time_duration(13,15,31)),
      537548,
      11.5438, 48.1353, 577,
      6, "A", "");

  ticksParsingTest("BLSEC,2E59FE,V,131532,200810,4808.1187,N,01132.6301,E,577.2000,M,06",
      pt::ptime(gr::date(2010,8,20),pt::time_duration(13,15,32)),
      3037694,
      11.5438, 48.1353, 577,
      6, "V", "");
}

void HardwareParsingTest::testTicksParsingF25() {
  ticksParsingTest("BS,11C2CC,A,084638,200311,4808.1313,N,01132.6202,E,532.8,09,27b",
      pt::ptime(gr::date(2011,3,20),pt::time_duration(8,46,38)),
      1163980,
      11.5436, 48.1355, 533,
      9, "A", "27b");

  ticksParsingTest("BS,37E912,V,084639,200311,4808.1313,N,01132.6201,W,532.9,08,27b",
      pt::ptime(gr::date(2011,3,20),pt::time_duration(8,46,39)),
      3664146,
      -11.5436, 48.1355, 533,
      8, "V", "27b");
}

void HardwareParsingTest::samplesParsingTest(const std::string& input, bo::hardware::gps::Base& gps, unsigned int counterValue, bo::data::Format format, const std::string& rawData) {
  std::vector<std::string> fields;

  bo::util::String::split(input, fields, ",");

  bo::hardware::parsing::Samples samplesParser(fields, gps);

  CPPUNIT_ASSERT_EQUAL(counterValue, samplesParser.getCounterValue());
  bo::data::Waveform::AP waveform = samplesParser.getWaveform();
  CPPUNIT_ASSERT_EQUAL(format, *(waveform->getArray().getFormat()));
  CPPUNIT_ASSERT_EQUAL(rawData, samplesParser.getRawData());
}

void HardwareParsingTest::testHexStringParsing()
{
  ParseBase parseBase;

  for (unsigned int i = 0; i <= 0xff; i++) {
    std::ostringstream oss;
    oss << std::hex;
    oss << i;

    CPPUNIT_ASSERT_EQUAL(i, parseBase.parseHexString(oss.str()));
  }
}

void HardwareParsingTest::testSamplesParsing() {
  std::vector<std::string> fields;
  
  bo::hardware::comm::Dummy comm;
  bo::hardware::gps::Dummy gps(comm);

  {
    bo::util::String::split("BS,37E912,A,084639,200311,4808.1313,N,01132.6201,E,532.9,09,27b", fields, ",");
    bo::hardware::parsing::Ticks ticksParser(fields);
    gps.set(ticksParser);
  }
  
  {
    bo::util::String::split("BS,5EE912,A,084640,200311,4808.1313,N,01132.6201,E,532.9,09,27b", fields, ",");
    bo::hardware::parsing::Ticks ticksParser(fields);
    gps.set(ticksParser);
  }
  
  samplesParsingTest("BLSIG,F133E6,CF4,860", gps, 15807462, bo::data::Format(12, 2, 1), "CF4860");
  //"BLSIG,F1341D,450,77C");

  samplesParsingTest("BLSEQ,F67E15,667E667D6A7E707F787F817F887F8C808F80907F8F809381968191808A7F847F7F7F737E647D587D507D487B467B4D7C587C647C747D887F9780A181A983AF84B084AD84AA83A782A0819A80957F907F8A7D847E807E7C7D777B747C727C727B717B717D727F737F75807682778279827A817A817B807C7E7C7C7A7B797B777A", gps, 16154133, bo::data::Format(8,2,64),"667E667D6A7E707F787F817F887F8C808F80907F8F809381968191808A7F847F7F7F737E647D587D507D487B467B4D7C587C647C747D887F9780A181A983AF84B084AD84AA83A782A0819A80957F907F8A7D847E807E7C7D777B747C727C727B717B717D727F737F75807682778279827A817A817B807C7E7C7C7A7B797B777A");

  //"BLSEQ,00D13B,4C7E1A8103831E834F7F7F7DB07EDE7DF37AF17BEC7FE580D17DB77EA3828D81747E6A7F6D81697F627C607E61805F7E617E69816F827280787F79807780767E7B7D797D727D6F7D747E757E727F73807D80837E847D867E877E827D7D7D7B7F777E737C737C767D767C7A7D817E8580858089818B818B828D819180917E927D"
}


