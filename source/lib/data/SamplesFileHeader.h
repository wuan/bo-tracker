#ifndef BLITZORTUNG_DATA_SAMPLESFILEHEADER_H_
#define BLITZORTUNG_DATA_SAMPLESFILEHEADER_H_

#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "Logger.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {

    //! file io helper class for data::Samples
    class SamplesFileHeader {

      private:
	
	//! static constant for file id
	static const char *FILEID;

	//! static constant for date which marks the start of the epoch
	static const gr::date STARTOFEPOCH;

	//! file date
	gr::date fileDate_;

	//! file version
	unsigned short fileVersion_;
	
	//! number of samples in file
	unsigned int numberOfSamples_;

	//! strage size of one sample
	unsigned int sampleSize_;

	//! sample creator object
	data::sample::Base::Creator::P sampleCreator_;

	//! class logger
	mutable Logger logger_;

	//! set creator according to file version
	void setCreator(const unsigned short);

      public:

	//! constructor
	SamplesFileHeader(const gr::date& fileDate = gr::date(gr::not_a_date_time), const unsigned short fileVersion = 0);

	//! destructor
	~SamplesFileHeader();

	//! read header from file
	void read(std::ifstream&);

	//! set file date
	void setDate(const gr::date&);

	//! get file date
	const gr::date& getDate() const;
	
	//! get number of samples in file
	unsigned int getNumberOfSamples() const;

	//! get storage size of one sample
	unsigned int getSampleSize() const;

	//! set file version
	void setFileVersion(const unsigned short);

	//! get file version
	const unsigned short getFileVersion() const;

	bool operator==(const SamplesFileHeader&);
	
	bool operator!=(const SamplesFileHeader&);

	//! set creator object depending on file version number
	sample::Base::AP createSample() const;

	std::string formatFilename(const std::string& fileformat) const;

	// check, if a file with given filename exists
	bool fileExists(const std::string&);

	//! read file header
	void read(const std::string&);
	
	//! write file header
	void write(const std::string&);
	
	unsigned int getSize() const;
    };

  }
}

#endif

