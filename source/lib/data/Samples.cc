#include "data/Samples.h"
#include "data/SamplesFile.h"

namespace blitzortung {
  namespace data {

    Samples::Samples()
    {
      init();
    }

    void Samples::init() {
      samples_ = Sample::VP(new Sample::V());
    }


    void Samples::add(sample::Base* sample) {
      if (samples_->size() == 0) {
	header_.setDate(sample->getTime().date());
      } else {
	if (header_.getDate() != sample->getTime().date())
	  throw exception::Base("sample date mismatch");
      }
      samples_->push_back(sample);
    }

    void Samples::add(std::auto_ptr<sample::Base> sample) {
      add(sample.release());
    }

    void Samples::add(sample::Base::V::auto_type sample) {
      add(sample.release());
    }

    void Samples::add(Samples&) throw(exception::Base) {
    }

    const gr::date& Samples::getDate() const {
      return header_.getDate();
    }

    void Samples::setDate(const gr::date& date) {
      header_.setDate(date);
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

    Samples::Sample::VI Samples::erase(Sample::VI start, Sample::VI end) {
      return samples_->erase(start, end);
    }

    const sample::Base& Samples::front() const {
      return samples_->front();
    }

    sample::Base& Samples::front() {
      return samples_->front();
    }

    const sample::Base& Samples::back() const {
      return samples_->back();
    }

    sample::Base& Samples::back() {
      return samples_->back();
    }

    Samples::Sample::CVI Samples::begin() const {
      return samples_->begin();
    }

    Samples::Sample::VI Samples::begin() {
      return samples_->begin();
    }

    Samples::Sample::CVI Samples::end() const {
      return samples_->end();
    }

    Samples::Sample::VI Samples::end() {
      return samples_->end();
    }

    std::string Samples::appendToFile(const std::string& fileName, const unsigned short fileVersion) {
      if (samples_->size() > 0) {

	if (fileVersion != 0) {
	  // override file version if fileVersion argument is not 0
	  header_.setFileVersion(fileVersion);
	} else {
	  if (header_.getFileVersion() == 0) {
	    // use version of first sample, if no file-version is set
	    header_.setFileVersion(samples_->front().getVersion());
	  }
	}

	SamplesFile samplesFile(fileName, header_);

	samplesFile.append(samples_);
	
	return header_.formatFilename(fileName);
      }
      return "";
    }

    std::string Samples::writeToFile(const std::string& fileName, const unsigned short fileVersion) {
      if (samples_->size() > 0) {

	if (fileVersion != 0) {
	  // override file version if fileVersion argument is not 0
	  header_.setFileVersion(fileVersion);
	} else {
	  if (header_.getFileVersion() == 0) {
	    // use version of first sample, if no file-version is set
	    header_.setFileVersion(samples_->front().getVersion());
	  }
	}
	SamplesFile samplesFile(fileName, header_);

	samplesFile.append(samples_);
	
	return header_.formatFilename(fileName);
      }
      return "";
    }

    void Samples::readFromFile(const std::string& filename, const pt::time_duration& startTime, const pt::time_duration& endTime) {
      SamplesFile samplesFile(filename, header_);
      samples_ = samplesFile.read(startTime, endTime);
    }

  }
}
