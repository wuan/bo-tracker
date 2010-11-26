
#include "data/SamplesFile.h"
#include "data/Sample.h"
#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    SamplesFile::SamplesFile(const std::string& name) :
      name_(name),
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

      const std::string filename = header_.formatFilename(name_);
      if (openFile) {
	fstream_.open(filename.c_str(), openmode);
	openmode_ = openmode;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "open() open file '" << filename << "' with mode " << openmode_;
      }
    }

    void SamplesFile::close() {
      if (isOpen()) {
	fstream_.close();
      }
    }


    SamplesFile::~SamplesFile() {
    }

    const SamplesHeader& SamplesFile::getHeader() const {
      return header_;
    }

    void SamplesFile::write(const Samples& samples) {
      writeSamples(name_, samples); 
    }

    void SamplesFile::append(const Samples& samples) {

      // check if file exists
      if (header_.fileExists(name_)) {
	writeSamples(name_, samples, true);
      } else {
	writeSamples(name_, samples);
      }

    }
    
    void SamplesFile::writeSamples(const std::string& name, const Samples& samples, bool append) {

      if (samples.size() > 0) {

	header_.setDate(samples.front().getWaveform().getTime().date());
	header_.setVersion(samples.front().getVersion());

	setFilename(name);

	if (append) {
	  SamplesHeader header;
	  header.read(getFilename());

	  if (header != header_)
	    throw exception::Base("data::SamplesFile() writeSamples() header mismatch during append");

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "writeSamples() append";
	} else {
	  header_.write(getFilename());
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "writeSamples() write header " << header_.getDate() << " v" << header_.getVersion();
	}

	open(std::ios::out | std::ios::binary | std::ios::app);

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "writeSamples() write " << samples.size() << " samples";

	for (Sample::CVI sample = samples.begin(); sample != samples.end(); sample++) {
	  sample->toStream(fstream_);
	}

	close();
      }
    }

    unsigned int SamplesFile::findSample(const pt::time_duration& target, unsigned int start, unsigned int end) {
      // determine middle of interval for next btree search iteration
      unsigned int middle = (start + end) / 2;

      // read sample
      fstream_.seekg(header_.getSize() + middle * header_.getSampleSize(), std::ios::beg);
      Sample::AP sample = header_.createSample(fstream_);

      // recursive btree search
      if (sample->getWaveform().getTime().time_of_day() > target) {
	// lower interval
	end = middle;
      } else {
	// upper interval
	start = middle;
      }

      // check, if search has reached last iteration
      if ((end - start) <= 1) {

	fstream_.seekg(header_.getSize() + start * header_.getSampleSize(), std::ios::beg);

	sample = header_.createSample(fstream_);

	// return index position
	if (sample->getWaveform().getTime().time_of_day() >= target) {
	  return start;
	} else {
	  return end;
	}
      } else {
	// do next iteration
	return findSample(target, start, end);
      }
    }
		
    Sample::VP SamplesFile::read(const pt::time_duration& start, const pt::time_duration& end) {

      Sample::VP samples(new Sample::V());
      
      header_.read(name_);
     
      open(std::ios::in | std::ios::binary);

      unsigned int startIndex = 0;
      unsigned int endIndex = header_.getNumberOfSamples();

      if (! start.is_not_a_date_time())
	startIndex = findSample(start, 0, endIndex);

      if (! end.is_not_a_date_time())
	endIndex = findSample(end, startIndex, endIndex);

      fstream_.seekg(header_.getSize() + startIndex * header_.getSampleSize() , std::ios::beg);

      unsigned int numberOfSamples = endIndex - startIndex;
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() read " << numberOfSamples << " samples (" << startIndex << " - " << endIndex - 1 << ")";

      for(unsigned int i=0; i < numberOfSamples; i++) {
	Sample::AP sample(header_.createSample(fstream_));

	samples->push_back(sample);
      }

      close();
      
      return samples;
    }
  }
}
