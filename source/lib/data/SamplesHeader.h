#ifndef BLITZORTUNG_DATA_SAMPLESHEADER_H_
#define BLITZORTUNG_DATA_SAMPLESHEADER_H_

#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "Logger.h"
#include "data/Sample.h"
#include "data/SampleFactory.h"
#include "data/sample/V1Factory.h"
#include "data/sample/V2Factory.h"

namespace blitzortung {
  namespace data {

    //! file io helper class for data::Samples
    class SamplesHeader {

      private:
	
	//! static constant for file id
	static const char *ID;

	//! static constant for date which marks the start of the epoch
	static const gr::date STARTOFEPOCH;

	//! file date
	gr::date date_;

	//! file version
	unsigned short version_;
	
	//! number of samples in file
	unsigned int numberOfSamples_;

	//! strage size of one sample
	unsigned int sampleSize_;

	//! sample creator
	SampleFactory::AP sampleFactory_;

	//! class logger
	mutable Logger logger_;

	//! set creator according to file version
	void setFactory();

      public:

	//! constructor
	SamplesHeader(const gr::date& date = gr::date(gr::not_a_date_time), const unsigned short version = 0);

	//! destructor
	~SamplesHeader();

	//! read header from stream
	void read(std::ifstream&);

	//! set header date
	void setDate(const gr::date&);

	//! get header date
	const gr::date& getDate() const;
	
	//! get number of samples in file
	unsigned int getNumberOfSamples() const;

	//! get storage size of one sample
	unsigned int getSampleSize() const;

	//! set header version
	void setVersion(const unsigned short);

	//! get header version
	const unsigned short getVersion() const;

	bool operator==(const SamplesHeader&);
	
	bool operator!=(const SamplesHeader&);

	//! set creator object depending on file version number
	Sample::AP createSample(std::iostream& stream) const;

	std::string formatFilename(const std::string& fileformat) const;

	// check, if a file with given filename exists
	bool fileExists(const std::string&) const;

	//! read file header
	void read(const std::string&);
	
	//! write file header
	void write(const std::string&) const;
	
	unsigned int getSize() const;
    };

  }
}

#endif

