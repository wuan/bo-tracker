#ifndef BLITZORTUNG_OUTPUT_FILE_H_
#define BLITZORTUNG_OUTPUT_FILE_H_

#include "output/Base.h"

namespace blitzortung {
  namespace output {

    class File : public Base {

      private:
	//! output file name
	const std::string& outputFile_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! create network transfer object
	File(const std::string&);

	//! delete nework transfer object
	virtual ~File();

	//! output data to file
	void output(data::Event::VP& events);

    };
  }
}

#endif
