#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/Event.h"
#include "test-lib-data-events.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( EventTest );

bo::data::Event::AP EventTest::getEvent(const pt::ptime& time) {

  bo::data::Array::AP array(new bo::data::Array(*dataFormat_));

  for (unsigned int i=0; i < 128; i++) {
    array->set(100 - i, i, 0);
    array->set(-50 + i, i, 1);
  }

  bo::data::Waveform::AP wfm(new bo::data::Waveform(array, time + pt::nanoseconds(3125) * 10, pt::nanoseconds(3125)));

  // create GpsInfo
  bo::data::GpsInfo::AP gpsInfo(new bo::data::GpsInfo());

  return bo::data::Event::AP(new bo::data::Event(wfm, gpsInfo));
}

bo::data::Events::P EventTest::getEvents1() {
  gr::date eventDate(2010,8,1);

  std::vector<pt::time_duration> eventTimes;
  eventTimes.push_back(pt::time_duration( 0, 0, 0,0));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,010));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,020));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,050));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,075));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,100));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,150));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,200));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,250));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,500));
  eventTimes.push_back(pt::time_duration( 0, 0, 0,750));
  eventTimes.push_back(pt::time_duration( 0, 0, 1,0));
  eventTimes.push_back(pt::time_duration( 0, 0,10,0));
  eventTimes.push_back(pt::time_duration( 0, 0,30,0));
  eventTimes.push_back(pt::time_duration( 0, 1,30,0));
  eventTimes.push_back(pt::time_duration( 0, 5,00,0));
  eventTimes.push_back(pt::time_duration(12,35,10,0));
  eventTimes.push_back(pt::time_duration(12,40,10,0));
  eventTimes.push_back(pt::time_duration(12,45,10,0));
  eventTimes.push_back(pt::time_duration(12,50,10,0));
  eventTimes.push_back(pt::time_duration(12,51,05,0));
  eventTimes.push_back(pt::time_duration(12,51,06,0));
  eventTimes.push_back(pt::time_duration(12,51,07,0));
  eventTimes.push_back(pt::time_duration(20,00,00,000));
  eventTimes.push_back(pt::time_duration(21,00,00,000));
  eventTimes.push_back(pt::time_duration(22,00,00,000));
  eventTimes.push_back(pt::time_duration(23,00,00,000));
  eventTimes.push_back(pt::time_duration(23,50,00,000));
  eventTimes.push_back(pt::time_duration(23,55,00,000));
  eventTimes.push_back(pt::time_duration(23,59,59,000));

  bo::data::Events::P events(new bo::data::Events());

  for (std::vector<pt::time_duration>::iterator eventTime = eventTimes.begin(); eventTime != eventTimes.end(); eventTime++) {
    pt::ptime eventDateTime(eventDate, *eventTime);
    bo::data::Event::AP event = getEvent(eventDateTime);

    events->add(event);
  }

  return events;
}

bo::data::Events::P EventTest::getEvents2() {
  bo::data::Events::P events(new bo::data::Events());

  return events;
}

void EventTest::tearDown() {

}

void EventTest::testAdd() {

  gr::date eventDate(2010,8,1);

  bo::data::Events events;

  events.add(getEvent(pt::ptime(eventDate, pt::time_duration(11,20,00))));
  events.add(getEvent(pt::ptime(eventDate, pt::time_duration(11,21,00))));
  events.add(getEvent(pt::ptime(eventDate, pt::time_duration(11,22,00))));
  events.add(getEvent(pt::ptime(eventDate, pt::time_duration(11,25,00))));
  events.add(getEvent(pt::ptime(eventDate, pt::time_duration(11,30,00))));

  CPPUNIT_ASSERT_THROW(events.add(getEvent(pt::ptime(eventDate, pt::time_duration(11,30,00)) + gr::days(1))), bo::exception::Base);

  /*for(bo::data::Events::I event=events.begin(); event != events.end(); event++) {
    std::cout << *event << std::endl;
  }*/
}

void EventTest::testWrite() {

  bo::data::Events::P events = getEvents1();

  int originalSize = events->size();

  events->writeToFile("UserName_%Y%m%d.bos");
  std::string fileName = events->writeToFile("UserName_%Y%m%d.bos");

  events->clear();

  CPPUNIT_ASSERT_EQUAL(events->size() , 0);

  events->readFromFile(fileName);

  CPPUNIT_ASSERT_EQUAL(events->size() , originalSize);

  events->readFromFile(fileName);

  CPPUNIT_ASSERT_EQUAL(events->size() , originalSize);
}

void EventTest::testAppend() {

  bo::data::Events::P events = getEvents1();

  int originalSize = events->size();

  std::string fileName = events->writeToFile("UserName_%Y%m%d.bos");

  events->readFromFile(fileName);

  CPPUNIT_ASSERT_EQUAL(events->size() , originalSize);

  events->appendToFile(fileName);

  events->readFromFile(fileName);

  CPPUNIT_ASSERT_EQUAL(events->size() , originalSize * 2);
}

void EventTest::testSize() {

  const unsigned int dataSize = getDataSize();

  gr::date eventDate(2010,8,1);

  bo::data::Event::AP event = getEvent(pt::ptime(eventDate, pt::time_duration(11,20,00)));

  std::stringstream ss;

  event->toStream(ss);

  CPPUNIT_ASSERT_EQUAL(dataSize, event->getSize());

  CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());

}
