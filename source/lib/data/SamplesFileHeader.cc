#include <fstream>

#include "data/SamplesFileHeader.h"
#include "data/sample/Base.h"
#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    const char* SamplesFileHeader::FILEID = "BOSF";

    const gr::date SamplesFileHeader::STARTOFEPOCH = gr::date(1970, 1, 1);

    SamplesFileHeader::SamplesFileHeader(const gr::date& fileDate, const unsigned short fileVersion) :
      fileDate_(fileDate),
      fileVersion_(fileVersion)
    {
    }

    SamplesFileHeader::~SamplesFileHeader() {
    }

    const gr::date& SamplesFileHeader::getDate() const {
      return fileDate_;
    }

    void SamplesFileHeader::setDate(const gr::date& fileDate) {
      fileDate_ = fileDate;
    }

    void SamplesFileHeader::setFileVersion(const unsigned short fileVersion) {
      fileVersion_ = fileVersion;
    }

    const unsigned short SamplesFileHeader::getFileVersion() const {
      return fileVersion_;
    }

    void SamplesFileHeader::read(const std::string& filename) {
      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::in | std::ios::binary);
      
      fstream.seekg(0, std::ios::beg);

      {
	char tempFileId[5];

	// check for correct file ID
	fstream.read(tempFileId, 4);
	
	std::string fileId(tempFileId);
	tempFileId[sizeof(tempFileId)-1] = 0;

	if (std::string(tempFileId) != FILEID) {
	  std::ostringstream oss;
	  oss << "data::SamplesFileHeader bad file header '" << tempFileId << "' vs '" << FILEID << "'";
	  throw exception::Base(oss.str());
	}
      }

      {
	unsigned int fileEpochDays;

	// read file version from file
	fstream.read((char*)&fileEpochDays, sizeof(unsigned int));
	fileDate_ = STARTOFEPOCH + gr::days(fileEpochDays);
      }
  
      // read file version from file
      fstream.read((char*)&fileVersion_, sizeof(fileVersion_));

      // set object to create sample objects 
      setCreator(fileVersion_);

    }

    std::string SamplesFileHeader::formatFilename(const std::string& fileformat) const {

      if (! fileDate_.is_not_a_date()) {

        gr::date_facet *datefacet = new gr::date_facet();
        datefacet->format(fileformat.c_str());

        std::ostringstream filenamestream;
        filenamestream.imbue(std::locale(std::locale::classic(), datefacet));
        filenamestream << fileDate_;

        return filenamestream.str();
      } else {
	return fileformat;
	throw exception::Base("data::SamplesFileHeader formatFilename() ERROR: no file date");
      }
    }

    void SamplesFileHeader::write(const std::string& filename) {

      if (fileVersion_ == 0)
	throw exception::Base("data::SamplesFileHeader writeHeader() invalid file version");

      if (fileDate_ == gr::date(pt::not_a_date_time))
	throw exception::Base("data::SamplesFileHeader writeHeader() invalid file date");

      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
      
      fstream.seekg(0, std::ios::beg);

      fstream.write(FILEID, 4);

      {
	unsigned int fileEpochDays = (fileDate_ - STARTOFEPOCH).days();
	fstream.write((char*) &fileEpochDays, sizeof(unsigned int));
      }

      fstream.write((char*) &fileVersion_, sizeof(unsigned short));
    }

    void SamplesFileHeader::setCreator(unsigned short fileVersion) {

      switch (fileVersion) {
	case 1:
	  sampleCreator_ = sample::Base::Creator::P(new sample::V1::Creator());
	  break;

	default:
	  std::ostringstream oss;
	  oss << "data::SamplesFileHeader setCreator() unknown file version " << fileVersion;
	  throw exception::Base(oss.str().c_str());
	  break;
      }
    }

    bool file_exists(std::string filename) {
      std::ifstream file;

      file.open(filename.c_str(), std::ios::in);
      file.close();

      return ! file.fail();
    }

    bool SamplesFileHeader::fileExists(const std::string& filename) {
      return file_exists(formatFilename(filename));
    }
    
    bool SamplesFileHeader::operator==(const SamplesFileHeader& other) {
      return fileDate_ == other.fileDate_ && fileVersion_ == other.fileVersion_;
    }
	
    bool SamplesFileHeader::operator!=(const SamplesFileHeader& other) {
      return !(*this == other);
    }


  }
}

