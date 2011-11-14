#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/MEvent.h"
#include "test-lib-data-events.h"

//CPPUNIT_TEST_SUITE_REGISTRATION( EventTest );

void EventTest::addEvent(bo::data::Events& events, const pt::ptime& time) {
  bo::data::Event::AP event(createEvent(time));
  events.add(event);
}

bo::data::Event::AP EventTest::createEvent(const pt::ptime& time) {
  return createEventWithFormat(dataFormat_, time);
}

bo::data::Event::AP EventTest::createEventWithFormat(bo::data::Format::CP dataFormat, const pt::ptime& time) {

  bo::data::Array::AP array(new bo::data::Array(dataFormat));

  int valueOffset[] = {100, -50};

  for (unsigned int sample=0; sample < dataFormat->getNumberOfSamples(); sample++) {
    for (unsigned int channel=0; channel < dataFormat->getNumberOfChannels(); channel++) {
      array->set(valueOffset[channel] + sample, sample, channel);
    }
  }

  pt::time_duration dt = pt::nanoseconds(0);

  if (dataFormat->getNumberOfSamples() > 1)
    dt = pt::nanoseconds(3125);

  bo::data::Waveform::AP wfm(new bo::data::Waveform(array, time + pt::nanoseconds(3125) * 10, dt));

  // create GpsInfo
  bo::data::GpsInfo::AP gpsInfo(new bo::data::GpsInfo());

  return bo::data::Event::AP(new bo::data::MEvent(wfm, gpsInfo, "n/a"));
}

bo::data::Events::P EventTest::createEvents1() {
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
    addEvent(*events, eventDateTime);
  }

  return events;
}

bo::data::Events::P EventTest::createEvents2() {
  bo::data::Events::P events(new bo::data::Events());

  return events;
}

void EventTest::tearDown() {

}

void EventTest::testAdd() {

  gr::date eventDate(2010,8,1);

  bo::data::Events events;

  addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,20,00))));
  addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,21,00))));
  addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,22,00))));
  addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,22,00))));
  addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,25,00))));
  addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,30,00))));

  CPPUNIT_ASSERT_THROW(addEvent(events, (pt::ptime(eventDate, pt::time_duration(11,30,00)) + gr::days(1))), bo::exception::Base);

  /*for(bo::data::Events::I event=events.begin(); event != events.end(); event++) {
    std::cout << *event << std::endl;
  }*/
}

void EventTest::testWrite() {

  bo::data::Events::P events = createEvents1();

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

  bo::data::Events::P events = createEvents1();

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

  bo::data::Event::AP event = createEvent(pt::ptime(eventDate, pt::time_duration(11,20,00)));

  std::stringstream ss;

  event->toStream(ss);

  CPPUNIT_ASSERT_EQUAL(dataSize, event->getSize());

  CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());
}
