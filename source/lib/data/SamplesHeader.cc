#include <fstream>

#include "data/SamplesHeader.h"
#include "data/sample/Base.h"
#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    const char* SamplesHeader::ID = "BOSF";

    const gr::date SamplesHeader::STARTOFEPOCH = gr::date(1970, 1, 1);

    bool file_exists(std::string filename) {
      std::ifstream file;

      file.open(filename.c_str(), std::ios::in);
      file.close();

      return ! file.fail();
    }

    SamplesHeader::SamplesHeader(const gr::date& date, const unsigned short version) :
      date_(date),
      version_(version),
      logger_("data.SamplesHeader")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "construct() date " << date << " version " << version;
    }

    SamplesHeader::~SamplesHeader() {
    }

    const gr::date& SamplesHeader::getDate() const {
      return date_;
    }

    void SamplesHeader::setDate(const gr::date& date) {
      date_ = date;
    }

    void SamplesHeader::setVersion(const unsigned short version) {
      version_ = version;
    }

    const unsigned short SamplesHeader::getVersion() const {
      return version_;
    }
    
    unsigned int SamplesHeader::getNumberOfSamples() const {
      return numberOfSamples_;
    }  

    unsigned int SamplesHeader::getSampleSize() const {
      return sampleSize_;
    }  

    void SamplesHeader::read(const std::string& filename) {
      
      if (! file_exists(filename)) {
	std::ostringstream oss;
	oss << "data::SampleHeader::read() file '" << filename << "' does not exist";
	throw exception::Base(oss.str());
      }
      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::in | std::ios::binary);
      
      fstream.seekg(0, std::ios::beg);

      {
	char tempId[5];

	// check for correct file ID
	fstream.read(tempId, 4);
	
	std::string fileId(tempId);
	tempId[sizeof(tempId)-1] = 0;

	if (std::string(tempId) != ID) {
	  std::ostringstream oss;
	  oss << "data::SamplesHeader bad file header '" << tempId << "' vs '" << ID << "'";
	  throw exception::Base(oss.str());
	}
      }

      {
	unsigned int fileEpochDays;

	// read file version from file
	fstream.read((char*)&fileEpochDays, sizeof(unsigned int));
	date_ = STARTOFEPOCH + gr::days(fileEpochDays);
      }

      // read file version from file
      fstream.read((char*)&version_, sizeof(version_));

  
      assert(fstream.tellg() == getSize());
      
      fstream.seekg(0, std::ios::end);
      unsigned int filesize = fstream.tellg();
      
      fstream.close();
      
      // set object to create sample objects 
      setCreator();

      sample::Base::AP dummySample = createSample();
      
      filesize -= getSize();

      sampleSize_ = dummySample->getSize();

      numberOfSamples_ = filesize / sampleSize_;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() date " << date_ << " version " << version_ << " #samples " << numberOfSamples_ << " samplesize " << sampleSize_ << " filesize " << filesize;

      if (numberOfSamples_ * dummySample->getSize() != filesize)
	throw exception::Base("data::SamplesHeader file size mismatch");
    }

    sample::Base::AP SamplesHeader::createSample() const {
      return sample::Base::AP((*sampleCreator_)());
    }
	
    std::string SamplesHeader::formatFilename(const std::string& fileformat) const {

      if (! date_.is_not_a_date()) {

        gr::date_facet *datefacet = new gr::date_facet();
        datefacet->format(fileformat.c_str());

        std::ostringstream filenamestream;
        filenamestream.imbue(std::locale(std::locale::classic(), datefacet));
        filenamestream << date_;

        return filenamestream.str();
      } else {
	return fileformat;
	throw exception::Base("data::SamplesHeader formatFilename() ERROR: no file date");
      }
    }

    void SamplesHeader::write(const std::string& filename) {

      if (version_ == 0)
	throw exception::Base("data::SamplesHeader writeHeader() invalid file version");

      if (date_ == gr::date(pt::not_a_date_time))
	throw exception::Base("data::SamplesHeader writeHeader() invalid file date");

      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
      
      fstream.seekg(0, std::ios::beg);

      fstream.write(ID, 4);

      {
	unsigned int fileEpochDays = (date_ - STARTOFEPOCH).days();
	fstream.write((char*) &fileEpochDays, sizeof(unsigned int));
      }

      fstream.write((char*) &version_, sizeof(unsigned short));
      
      assert(fstream.tellg() == getSize());
      
      fstream.close();
    }

    void SamplesHeader::setCreator() {

      switch (version_) {
	case 1:
	  sampleCreator_ = sample::Base::Creator::P(new sample::V1::Creator());
	  break;

	default:
	  std::ostringstream oss;
	  oss << "data::SamplesHeader setCreator() unknown file version " << version_;
	  throw exception::Base(oss.str().c_str());
	  break;
      }
    }


    bool SamplesHeader::fileExists(const std::string& filename) {
      return file_exists(formatFilename(filename));
    }
    
    bool SamplesHeader::operator==(const SamplesHeader& other) {
      return date_ == other.date_ && version_ == other.version_;
    }
	
    bool SamplesHeader::operator!=(const SamplesHeader& other) {
      return !(*this == other);
    }

    unsigned int SamplesHeader::getSize() const {
      return 10;
    }
    
  }
}

