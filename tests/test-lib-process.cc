#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "hardware/Pcb.h"

#include "data/MEvent.h"

#include "Process.h"
#include "network/transfer/None.h"
#include "output/None.h"

#include "test-lib-process.h"
#include "test-lib-data-events.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ProcessTest );

void ProcessTest::setUp() {
}

void ProcessTest::tearDown() {
}

void pushEvent(bo::Process& process, bo::data::Event::AP event)
{
  process.push(std::move(event));
}

void ProcessTest::testAroundMidnight() {
  bo::network::transfer::None transfer;
  bo::output::None output;

  bo::Process process(transfer, 1, output);

  bo::data::Format dataFormat(12,2,1);

  sleep(2);
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,10), pt::time_duration(23,59,56))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,10), pt::time_duration(23,59,57))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,10), pt::time_duration(23,59,58))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,10), pt::time_duration(23,59,59))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,11), pt::time_duration(0,0,0))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,11), pt::time_duration(0,0,1))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,11), pt::time_duration(0,0,2))));
  pushEvent(process, EventTest::createEventWithFormat(dataFormat, pt::ptime(gr::date(2002,1,11), pt::time_duration(0,0,3))));

  sleep(10);
}

