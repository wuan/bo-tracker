#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "hardware/pcb/Base.h"
#include "hardware/pcb/V4.h"
#include "hardware/pcb/V6.h"

#include "hardware/comm/Dummy.h"
#include "hardware/gps/Sirf.h"
#include "data/sample/V1.h"
#include "data/sample/V1Factory.h"

#include "test-lib-hardware-pcb.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HardwareTest );

void HardwareTest::setUp() {
}

void HardwareTest::tearDown() {
}

void HardwareTest::testV4() {
  bo::hardware::comm::Dummy comm;

  comm.setBaudRate(19200);

  comm.addReceivedLine("BLSEC,185630,190810,A,4729.2852,N,01904.2683,E,AB852D");
  comm.addReceivedLine("BLSEC,185631,190810,A,4729.2856,N,01904.2688,E,D1ABEA");
  comm.addReceivedLine("BLSIG,F133E6,CF4,860");
  comm.addReceivedLine("BLSIG,F1341D,450,77C");

  bo::hardware::gps::Sirf gps(comm);

  bo::data::sample::V1Factory sampleFactory;

  bo::hardware::pcb::V4 pcb(comm, gps, sampleFactory);

  int count = 0;
  while (pcb.isOpen()) {

    bo::data::Sample::AP sample = pcb.read();

    if (sample.get() != 0) {
      //std::cout << *sample << std::endl;
      count++;
    }

  }
  CPPUNIT_ASSERT_EQUAL(2, count);
}

void HardwareTest::testV6() {
  bo::hardware::comm::Dummy comm;

  comm.setBaudRate(19200);

  comm.addReceivedLine("BLSEC,0833CC,A,131531,200810,4808.1189,N,01132.6299,E,576.5000,M,06");
  comm.addReceivedLine("BLSEC,2E59FE,A,131532,200810,4808.1187,N,01132.6301,E,577.2000,M,06");
  comm.addReceivedLine("BLSEC,54802F,A,131533,200810,4808.1186,N,01132.6302,E,577.6000,M,06");
  comm.addReceivedLine("BLSEC,7AA661,A,131534,200810,4808.1185,N,01132.6303,E,577.7000,M,06");
  comm.addReceivedLine("BLSEC,A0CC93,A,131535,200810,4808.1182,N,01132.6305,E,578.6000,M,06");
  comm.addReceivedLine("BLSEC,C6F2C4,A,131536,200810,4808.1180,N,01132.6307,E,579.3000,M,06");
  comm.addReceivedLine("BLSEC,ED18F6,A,131537,200810,4808.1179,N,01132.6309,E,579.8000,M,06");
  comm.addReceivedLine("BLSEQ,F67E15,667E667D6A7E707F787F817F887F8C808F80907F8F809381968191808A7F847F7F7F737E647D587D507D487B467B4D7C587C647C747D887F9780A181A983AF84B084AD84AA83A782A0819A80957F907F8A7D847E807E7C7D777B747C727C727B717B717D727F737F75807682778279827A817A817B807C7E7C7C7A7B797B777A");
  comm.addReceivedLine("BLSEQ,00D13B,4C7E1A8103831E834F7F7F7DB07EDE7DF37AF17BEC7FE580D17DB77EA3828D81747E6A7F6D81697F627C607E61805F7E617E69816F827280787F79807780767E7B7D797D727D6F7D747E757E727F73807D80837E847D867E877E827D7D7D7B7F777E737C737C767D767C7A7D817E8580858089818B818B828D819180917E927D");

  bo::hardware::gps::Sirf gps(comm);

  bo::data::sample::V1Factory sampleFactory;

  bo::hardware::pcb::V6 pcb(comm, gps, sampleFactory);

  int count = 0;
  while (pcb.isOpen()) {

    bo::data::Sample::AP sample = pcb.read();

    if (sample.get() != 0) {
      //std::cout << *sample << std::endl;
      count++;
    }

  }
  CPPUNIT_ASSERT_EQUAL(2, count);
}


