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


    void File::output(data::Sample::VP& samples) {

      data::Samples outputSamples;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "saveData()";

      // move all current samples to
      for (data::Sample::VI sample = samples->begin(); sample != samples->end();) {

	if (outputSamples.size() != 0 &&
	    outputSamples.getDate() != sample->getWaveform().getTime().date()) {
	  outputSamples.appendToFile(outputFile_);
	  outputSamples.clear();
	}


	outputSamples.add(samples->release(sample));
      }

      if (outputSamples.size() > 0) {
	outputSamples.appendToFile(outputFile_);
      }
    }

  }
}
