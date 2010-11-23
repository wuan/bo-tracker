#ifndef BLITZORTUNG_OUTPUT_FILE_H_
#define BLITZORTUNG_OUTPUT_FILE_H_

#include "output/Base.h"

namespace blitzortung {
  namespace output {

    class File : public Base {

      private:
	//! output file name
	const std::string& outputFile_;

	//! sample creator
	const data::sample::Base::Creator& sampleCreator_;

	//! vector of samples
	data::sample::Base::VP samples_;
	
	//! logger for this class
	mutable Logger logger_;

      public:

	//! create network transfer object
	File(const std::string&, const data::sample::Base::Creator&);

	//! delete nework transfer object
	virtual ~File();

	//! output data to file
	void output(data::Sample::VP& samples);

    };
  }
}

#endif
