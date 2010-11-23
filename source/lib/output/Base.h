#ifndef BLITZORTUNG_OUTPUT_BASE_H_
#define BLITZORTUNG_OUTPUT_BASE_H_

#include "data/Samples.h"
#include "Logger.h"

namespace blitzortung {
  namespace output {

    class Base {

      public:

	typedef std::auto_ptr<Base> AP;

	//! output data to file
	virtual void output(data::Sample::VP& samples) = 0;

    };
  }
}

#endif
