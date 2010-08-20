#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "hardware/pcb/Base.h"
#include "hardware/pcb/V4.h"
#include "hardware/pcb/V6.h"

#include "hardware/comm/Dummy.h"
#include "hardware/gps/Sirf.h"
#include "data/sample/V1.h"

#include "test-lib-hardware-pcb.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HardwareTest );

void HardwareTest::setUp() {
}

void HardwareTest::tearDown() {
}

void HardwareTest::testV4() {
  bo::hardware::comm::Dummy comm;

  comm.setBaudRate(19200);

  comm.addReceivedLine("$BLSEC,185630,190810,A,4729.2852,N,01904.2683,E,AB852D*57");
  comm.addReceivedLine("$BLSEC,185631,190810,A,4729.2856,N,01904.2688,E,D1ABEA*53");
  comm.addReceivedLine("$BLSIG,F133E6,CF4,860*74");
  comm.addReceivedLine("$BLSIG,F1341D,450,77C*08");

  bo::hardware::gps::Sirf gps(comm);

  bo::hardware::pcb::V4 pcb(comm, gps, bo::data::sample::V1::Creator());

  while (pcb.isOpen()) {

    bo::data::sample::Base::AP sample = pcb.read();

    if (sample.get() != 0) {
      std::cout << *sample << std::endl;
    }

  }
}

void HardwareTest::testV6() {
  bo::hardware::comm::Dummy comm;

  comm.setBaudRate(19200);

//  comm.addReceivedLine("$BLSEC,185630,190810,A,4729.2852,N,01904.2683,E,AB852D*51");

  bo::hardware::gps::Sirf gps(comm);

  bo::hardware::pcb::V6 pcb(comm, gps, bo::data::sample::V1::Creator());

  while (pcb.isOpen()) {

    bo::data::sample::Base::AP sample = pcb.read();

    if (sample.get() != 0) {
      std::cout << *sample << std::endl;
    }

  }
}


