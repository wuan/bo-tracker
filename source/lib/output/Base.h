#ifndef BLITZORTUNG_OUTPUT_BASE_H_
#define BLITZORTUNG_OUTPUT_BASE_H_

#include "data/Events.h"
#include "Logger.h"

namespace blitzortung {
  namespace output {

    class Base {

      public:

	typedef std::auto_ptr<Base> AP;

	virtual ~Base() {}

	//! output data to file
	virtual void output(const data::Events& samples) = 0;

    };
  }
}

#endif
