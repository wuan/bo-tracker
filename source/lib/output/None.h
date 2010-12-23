#ifndef BLITZORTUNG_OUTPUT_NONE_H_
#define BLITZORTUNG_OUTPUT_NONE_H_

#include "output/Base.h"

namespace blitzortung {
  namespace output {

    class None : public Base {

      private:

	//! logger for this class
	mutable Logger logger_;

      public:

	//! create network transfer object
	None();

	//! delete nework transfer object
	virtual ~None();

	//! output data to file
	void output(data::Event::VP& events);

    };
  }
}

#endif
