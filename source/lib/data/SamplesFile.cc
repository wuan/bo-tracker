
#include "data/SamplesFile.h"
#include "data/sample/Base.h"
#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    SamplesFile::SamplesFile(const std::string& name, const SamplesFileHeader& header) :
      name_(name),
      header_(header),
      logger_("data.SamplesFile")
    {
    }

    bool SamplesFile::isOpen() const {
      return fstream_.is_open();
    }

    void SamplesFile::setFilename(const std::string& name) {
      name_ = name;
    }

    const std::string SamplesFile::getFilename() const {
      return header_.formatFilename(name_);
    }

    void SamplesFile::open(std::ios_base::openmode openmode) {

      bool openFile = false;

      if (isOpen()) {
	if (openmode != openmode_) {
	  close();
	  openFile = true;
	}
      } else {
	openFile = true;
      }

      if (openFile) {
	fstream_.open(name_.c_str(), openmode);
	openmode_ = openmode;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "open() open file '" << name_ << "' with mode " << openmode_;
      }
    }

    void SamplesFile::close() {
      if (isOpen()) {
	fstream_.close();
      }
    }


    SamplesFile::~SamplesFile() {
    }

    void SamplesFile::append(sample::Base::VP samples) {

      // check if file exists
      if (header_.fileExists(name_)) {
	writeSamples(name_, samples, true);
      } else {
	writeSamples(name_, samples);
      }

    }
    
    void SamplesFile::writeSamples(const std::string& name, sample::Base::VP samples, bool append) {
     
      setFilename(name);

      if (append) {
	SamplesFileHeader header;
	header.read(getFilename());
        
	if (header != header_)
	  throw exception::Base("data::SamplesFile() writeSamples() header mismatch during append");
      } else {
	header_.write(getFilename());
      }
      
      open(std::ios::out | std::ios::binary | std::ios::app);

      for (sample::Base::CVI sample = samples->begin(); sample != samples->end(); sample++) {
	sample->toStream(fstream_);
      }

      close();
    }

    unsigned int SamplesFile::findSample(const data::sample::Base::AP& tmpSample, const pt::time_duration& target, unsigned int start, unsigned int end) {
      // determine middle of interval for next btree search iteration
      unsigned int middle = (start + end) / 2;

      // read sample
      fstream_.seekg(header_.getSize() + middle * tmpSample->getSize(), std::ios::beg);
      tmpSample->fromStream(fstream_, header_.getDate());

      // recursive btree search
      if (tmpSample->getTime().time_of_day() > target) {
	// lower interval
	end = middle;
      } else {
	// upper interval
	start = middle;
      }

      // check, if search has reached last iteration
      if ((end - start) <= 1) {

	fstream_.seekg(header_.getSize() + start * tmpSample->getSize(), std::ios::beg);

	tmpSample->fromStream(fstream_, header_.getDate());

	// return index position
	if (tmpSample->getTime().time_of_day() > target) {
	  return start;
	} else {
	  return end;
	}
      } else {
	// do next iteration
	return findSample(tmpSample, target, start, end);
      }
    }
		
    sample::Base::VP SamplesFile::read(const pt::time_duration& start, const pt::time_duration& end) {

      sample::Base::VP samples(new sample::Base::V());
      
      header_.read(name_);
     
      open(std::ios::in | std::ios::binary);

      data::sample::Base::AP tmpSample(header_.createSample());

      unsigned int startIndex = 0;
      if (start.is_not_a_date_time())
	startIndex = findSample(tmpSample, start, 0, header_.getNumberOfSamples());

      unsigned int endIndex = 0;
      if (end.is_not_a_date_time())
	endIndex = findSample(tmpSample, end, startIndex, header_.getNumberOfSamples());

      fstream_.seekg(header_.getSize() + startIndex * header_.getSampleSize() , std::ios::beg);
      
      for(unsigned int i=0; i < header_.getNumberOfSamples(); i++) {
	sample::Base::AP sample(header_.createSample());

	sample->fromStream(fstream_, header_.getDate());

	samples->push_back(sample);
      }

      close();
      
      return samples;
    }
  }
}
