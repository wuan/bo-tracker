#ifndef BLITZORTUNG_DATA_SAMPLESFILE_H_
#define BLITZORTUNG_DATA_SAMPLESFILE_H_

#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "data/Samples.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {

    class SamplesFile : public Samples {

      private:
	
	static const char *FILEID;

	static const gr::date STARTOFEPOCH;

	//! file stream object
	std::fstream fstream_;

	//! file name
	std::string filename_;

	//! file open mode
	std::ios_base::openmode openmode_;

	//! sample creator object
	data::sample::Base::Creator::P sampleCreator_;

	//! read file header
	void readHeader();

	//! write file header
	void writeHeader();

	//! set creator object depending on file version number
	void setCreator(unsigned short);

      public:
	//! constructor
	SamplesFile();

	//! destructor
	~SamplesFile();

	//! set mode for opening a file
	void setOpenMode(std::ios_base::openmode);

	//! set Filename
	void setFilename(const std::string&);

	bool isOpen() const;

	void open();

	void close();

	//! read a single sample
	sample::Base::AP readSample();
    };

  }
}

#endif

