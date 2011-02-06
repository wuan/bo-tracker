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

      if (events.size() > 0) {
	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "output() append " << events.size() << " events to file " << outputFile_;

	events.appendToFile(outputFile_);
      }
    }

  }
}
