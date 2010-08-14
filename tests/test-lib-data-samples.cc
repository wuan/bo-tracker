#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "data/sample/V1.h"
#include "test-lib-data-samples.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SampleTest );

bo::data::sample::Base::AP SampleTest::getSample() {
  return bo::data::sample::Base::AP((*sampleCreator_)());
}

bo::data::sample::Base::AP SampleTest::getSample(const pt::ptime& time) {
  bo::data::sample::Base::AP sample = getSample();
  sample->setTime(time);
  return sample;
}

bo::data::Samples::P SampleTest::getSamples1() {
  gr::date sampleDate(2010,8,1);

  std::vector<pt::time_duration> sampleTimes;
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,0));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,010));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,020));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,050));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,075));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,100));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,150));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,200));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,250));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,500));
  sampleTimes.push_back(pt::time_duration( 0, 0, 0,750));
  sampleTimes.push_back(pt::time_duration( 0, 0, 1,0));
  sampleTimes.push_back(pt::time_duration( 0, 0,10,0));
  sampleTimes.push_back(pt::time_duration( 0, 0,30,0));
  sampleTimes.push_back(pt::time_duration( 0, 1,30,0));
  sampleTimes.push_back(pt::time_duration( 0, 5,00,0));
  sampleTimes.push_back(pt::time_duration(12,35,10,0));
  sampleTimes.push_back(pt::time_duration(12,40,10,0));
  sampleTimes.push_back(pt::time_duration(12,45,10,0));
  sampleTimes.push_back(pt::time_duration(12,50,10,0));
  sampleTimes.push_back(pt::time_duration(12,51,05,0));
  sampleTimes.push_back(pt::time_duration(12,51,06,0));
  sampleTimes.push_back(pt::time_duration(12,51,07,0));
  sampleTimes.push_back(pt::time_duration(20,00,00,000));
  sampleTimes.push_back(pt::time_duration(21,00,00,000));
  sampleTimes.push_back(pt::time_duration(22,00,00,000));
  sampleTimes.push_back(pt::time_duration(23,00,00,000));
  sampleTimes.push_back(pt::time_duration(23,50,00,000));
  sampleTimes.push_back(pt::time_duration(23,55,00,000));
  sampleTimes.push_back(pt::time_duration(23,59,59,000));

  bo::data::Samples::P samples(new bo::data::Samples());

  for (std::vector<pt::time_duration>::iterator sampleTime = sampleTimes.begin(); sampleTime != sampleTimes.end(); sampleTime++) {
    pt::ptime sampleDateTime(sampleDate, *sampleTime);
    bo::data::sample::Base::AP sample((*sampleCreator_)());

    sample->setTime(sampleDateTime);
    sample->setAmplitude(1, 1.0, 0.0);

    samples->add(sample);
  }

  return samples;
}

bo::data::Samples::P SampleTest::getSamples2() {
  bo::data::Samples::P samples(new bo::data::Samples());

  return samples;
}

void SampleTest::setUp() {
  sampleCreator_ = boost::shared_ptr<bo::data::sample::Base::Creator>(new bo::data::sample::V1::Creator());
}

void SampleTest::tearDown() {

}

void SampleTest::testAdd() {

  gr::date sampleDate(2010,8,1);

  bo::data::Samples samples;

  samples.add(getSample(pt::ptime(sampleDate, pt::time_duration(11,20,00))));
  samples.add(getSample(pt::ptime(sampleDate, pt::time_duration(11,21,00))));
  samples.add(getSample(pt::ptime(sampleDate, pt::time_duration(11,22,00))));
  samples.add(getSample(pt::ptime(sampleDate, pt::time_duration(11,25,00))));
  samples.add(getSample(pt::ptime(sampleDate, pt::time_duration(11,30,00))));


  CPPUNIT_ASSERT_THROW(samples.add(getSample(pt::ptime(sampleDate, pt::time_duration(11,30,00)) + gr::days(1))), bo::exception::Base);

  std::string fileName = samples.appendToFile("UserName_%Y%m%d.bos");

  samples.readFromFile(fileName);

  for(bo::data::Samples::I sample=samples.begin(); sample != samples.end(); sample++) {
    std::cout << *sample << std::endl;
  }
}


