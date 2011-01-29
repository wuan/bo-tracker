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

    void None::output(data::Events& eventsForOutput) {

      for (data::Event::VI event = eventsForOutput.begin(); event != eventsForOutput.end(); ) {
	//eventsForOutput->erase(event);
      }

    }

  }
}
