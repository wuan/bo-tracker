
#include "data/SamplesFile.h"
#include "data/sample/Base.h"
#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    SamplesFile::SamplesFile(const std::string& filename, const SamplesFileHeader& fileHeader) :
      filename_(filename),
      fileHeader_(fileHeader)
    {
    }

    SamplesFile::~SamplesFile() {
    }

    void SamplesFile::append(sample::Base::VP samples) {

      // check if file exists
      if (fileHeader_.fileExists(filename_)) {
	writeSamples(filename_, samples, true);
      } else {
	writeSamples(filename_, samples);
      }

    }
    
    void SamplesFile::writeSamples(const std::string& filename, sample::Base::VP samples, bool append) {
      
      if (append) {
	SamplesFileHeader fileHeader;
	fileHeader.read(fileHeader_.formatFilename(filename));
        
	if (fileHeader != fileHeader_)
	  throw exception::Base("data::SamplesFile() writeSamples() header mismatch during append");
      } else {
	fileHeader_.write(filename);
      }
      
      std::fstream fstream;

      fstream.open(fileHeader_.formatFilename(filename).c_str(), std::ios::out | std::ios::binary | std::ios::app);

      for (sample::Base::CVI sample = samples->begin(); sample != samples->end(); sample++) {
	sample->toStream(fstream);
      }

      fstream.close();
    }

    sample::Base::VP SamplesFile::read() {
      fileHeader_.read(filename_);
      
      
    }
  }
}

