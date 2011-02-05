#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include "exception/Base.h"
#include "output/File.h"

namespace blitzortung {
  namespace output {

    File::File(const std::string& outputFile)
      :
      outputFile_(outputFile),
      logger_("output.File")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "initalized()";
    }

    File::~File() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "deleted";
    }


    void File::output(const data::Events& events) {
/*      data::Events outputEvents(events.getDate(), events.getDataFormat());

      // move all current events to
      for (data::Event::VI event = events.begin(); event != events.end();) {

	if (outputEvents.size() != 0 &&
	    outputEvents.getDate() != event->getWaveform().getTime().date()) {
	  outputEvents.appendToFile(outputFile_);
	  outputEvents.clear();
	}

	outputEvents.add(events.release(event));
      }*/

      if (events.size() > 0) {
	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "output() append " << events.size() << " events to file " << outputFile_;

	events.appendToFile(outputFile_);
      }
    }

  }
}
