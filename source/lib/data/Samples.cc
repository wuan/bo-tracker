#include "data/Samples.h"
#include "data/SamplesFile.h"

namespace blitzortung {
  namespace data {

    Samples::Samples() :
      samples_(new Sample::V())
    {
    }

    void Samples::add(Sample* sample) {
      if (samples_->size() == 0) {
	date_ = sample->getWaveform().getTime().date();
      } else {
	if (date_ != sample->getWaveform().getTime().date())
	  throw exception::Base("sample date mismatch");
      }
      samples_->push_back(sample);
    }

    void Samples::add(Sample::AP sample) {
      add(sample.release());
    }

    void Samples::add(Sample::V::auto_type sample) {
      add(sample.release());
    }

    void Samples::add(Samples&) throw(exception::Base) {
    }

    const gr::date& Samples::getDate() const {
      return date_;
    }

    void Samples::setDate(const gr::date& date) {
      if (samples_->size() == 0) {
	date_ = date;
      } else {
	throw exception::Base("data::Samples::setDate() setting of date not allowed");
      }
    }

    int Samples::size() const {
      return samples_->size();
    }

    void Samples::sort() {
      samples_->sort();
    }

    void Samples::clear() {
      samples_->clear();
    }

    Sample::VI Samples::erase(Sample::VI start, Sample::VI end) {
      return samples_->erase(start, end);
    }

    const Sample& Samples::front() const {
      return samples_->front();
    }

    Sample& Samples::front() {
      return samples_->front();
    }

    const Sample& Samples::back() const {
      return samples_->back();
    }

    Sample& Samples::back() {
      return samples_->back();
    }

    Sample::CVI Samples::begin() const {
      return samples_->begin();
    }

    Sample::VI Samples::begin() {
      return samples_->begin();
    }

    Sample::CVI Samples::end() const {
      return samples_->end();
    }

    Sample::VI Samples::end() {
      return samples_->end();
    }

    std::string Samples::appendToFile(const std::string& fileName, const unsigned short fileVersion) {
      if (samples_->size() > 0) {

	SamplesFile samplesFile(fileName);

	samplesFile.append(*this);
	
	return samplesFile.getFilename();
      }
      return "";
    }

    std::string Samples::writeToFile(const std::string& fileName, const unsigned short fileVersion) {
      if (samples_->size() > 0) {

	SamplesFile samplesFile(fileName);

	samplesFile.write(*this);
	
	return samplesFile.getFilename();
      }
      return "";
    }

    void Samples::readFromFile(const std::string& filename, const pt::time_duration& startTime, const pt::time_duration& endTime) {

      SamplesFile samplesFile(filename);
      samples_ = samplesFile.read(startTime, endTime);
      date_ = samplesFile.getHeader().getDate();
    }

  }
}
