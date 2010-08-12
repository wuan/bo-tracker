
#include "data/SamplesFile.h"
#include "data/sample/Base.h"
#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    const char* SamplesFile::FILEID = "BOSF";

    const gr::date SamplesFile::STARTOFEPOCH = gr::date(1970, 1, 1);

    SamplesFile::SamplesFile() :
      openmode_(std::ios::in)
    {
    }

    SamplesFile::~SamplesFile() {
      // close file stream
      close();
    }

    void SamplesFile::setOpenMode(std::ios_base::openmode openmode) {
      openmode_ = openmode;
    }

    bool SamplesFile::isOpen() const {
      return fstream_.is_open();
    }

    void SamplesFile::open() {
      if (! isOpen()) {
	// open file stream
	fstream_.open(filename_.c_str(), openmode_ | std::ios::binary);
      }
    }

    void SamplesFile::close() {
      if (isOpen()) {
	fstream_.close();
      }
    }

    void SamplesFile::readHeader() {
      fstream_.seekg(0, std::ios::beg);

      {
	char tempFileId[5];

	// check for correct file ID
	fstream_.read(tempFileId, 4);

	tempFileId[4]=0;

	if (tempFileId != FILEID) {
	  std::ostringstream oss;
	  oss << "data::SamplesFile bad file header '" << tempFileId << "'";
	  throw exception::Base(oss.str());
	}

      }

      {
	unsigned int fileEpochDays;

	// read file version from file
	fstream_.read((char*)&fileEpochDays, sizeof(unsigned int));
      }

      {
	unsigned short fileVersion;
	// read file version from file
	fstream_.read((char*)&fileVersion, sizeof(unsigned short));

	// set object to create sample objects 
	setCreator(fileVersion);
      }

    }

    void SamplesFile::writeHeader() {
      fstream_.seekg(0, std::ios::beg);

      fstream_.write(FILEID, 4);

      {
	unsigned int fileEpochDays = (getDate() - STARTOFEPOCH).days();
	fstream_.write((char*) &fileEpochDays, sizeof(unsigned int));
      }

      {
	unsigned short fileVersion = 0; // TODO
	fstream_.write((char*) &fileVersion, sizeof(unsigned short));
      }
    }

    void SamplesFile::setCreator(unsigned short fileVersion) {

      switch (fileVersion) {
	case 1:
	  sampleCreator_ = sample::Base::Creator::P(new sample::V1::Creator());
	  break;

	default:
	  std::ostringstream oss;
	  oss << "data::SamplesFile setCreator() unknown file version " << fileVersion;
	  throw exception::Base(oss.str().c_str());
	  break;
      }
    }

  }
}

