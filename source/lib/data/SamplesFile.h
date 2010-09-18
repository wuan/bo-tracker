#ifndef BLITZORTUNG_DATA_SAMPLESFILE_H_
#define BLITZORTUNG_DATA_SAMPLESFILE_H_

#include <iostream>
#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "Logger.h"
#include "data/Samples.h"
#include "data/SamplesHeader.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {

    //! file io helper class for data::Samples
    class SamplesFile {

      private:
	
	//! file stream object
	std::fstream fstream_;

	//! file stream openmode
	std::ios_base::openmode openmode_;

	//! file name
	std::string name_;

	//! header information
	SamplesHeader header_;

	//! logger for class
	mutable Logger logger_;

	//! read a single sample
	sample::Base::AP readSample();

	//! append/write samples to file with given filename
	void writeSamples(const std::string&, sample::Base::VP, bool append=false);

	//! open the fstream
	void open(std::ios_base::openmode);

	//! check if fstream is open
	bool isOpen() const;

	//! close the fstream
	void close();

        //! do btree search for sample position
	unsigned int findSample(const data::sample::Base::AP&, const pt::time_duration& target, unsigned int start, unsigned int end);

      public:

	//! constructor
	SamplesFile(const std::string&, const SamplesHeader&);

	//! destructor
	~SamplesFile();

	//! set Filename
	void setFilename(const std::string&);

	//! get Filename
	const std::string getFilename() const;

	//! append or create a new file
	void append(sample::Base::VP);
	
	//! overwrite or create a new file
	void write(sample::Base::VP);
	
	//! read time range from file
	sample::Base::VP read(const pt::time_duration& start = pt::time_duration(pt::not_a_date_time), const pt::time_duration& end = pt::time_duration(pt::not_a_date_time));
    };

  }
}

#endif

