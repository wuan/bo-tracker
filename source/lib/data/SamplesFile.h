#ifndef BLITZORTUNG_DATA_SAMPLESFILE_H_
#define BLITZORTUNG_DATA_SAMPLESFILE_H_

#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "data/Samples.h"
#include "data/SamplesFileHeader.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {

    //! file io helper class for data::Samples
    class SamplesFile {

      private:
	
	//! file stream object
	std::fstream fstream_;

	//! file name
	std::string filename_;

	//! header information
	SamplesFileHeader fileHeader_;

	//! read a single sample
	sample::Base::AP readSample();

	//! append/write samples to file with given filename
	void writeSamples(const std::string&, sample::Base::VP, bool append=false);

      public:
	//! constructor
	SamplesFile(const std::string&, const SamplesFileHeader&);

	//! destructor
	~SamplesFile();

	//! set Filename
	void setFilename(const std::string&);

	//! append or create a new file
	void append(sample::Base::VP);
	
	//! overwrite or create a new file
	void write(sample::Base::VP);
	
	//! overwrite or create a new file
	sample::Base::VP read();
    };

  }
}

#endif

