#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include "exception/Base.h"
#include "output/None.h"

namespace blitzortung {
  namespace output {

    None::None() :
      logger_("output.None")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "initalized()";
    }

    None::~None() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "deleted";
    }

    void None::output(data::Sample::VP& samplesForOutput) {

      for (data::Sample::VI sample = samplesForOutput->begin(); sample != samplesForOutput->end(); ) {
	samplesForOutput->erase(sample);
      }

    }

  }
}
