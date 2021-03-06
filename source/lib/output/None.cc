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

    void None::output(const data::Events& eventsForOutput) {

      for (data::Event::CVI event = eventsForOutput.begin(); event != eventsForOutput.end(); event++) {
	//eventsForOutput->erase(event);
      }

    }

  }
}
