#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include "exception/Base.h"
#include "output/File.h"

namespace blitzortung {
  namespace output {

    File::File(const std::string& outputFile, const data::sample::Base::Creator& sampleCreator)
      :
      outputFile_(outputFile),
      sampleCreator_(sampleCreator),
      samples_(new data::sample::Base::V()),
      logger_("output.File")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "initalized()";
    }

    File::~File() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "deleted";
    }


    void File::output(data::Sample::VP& samplesForOutput) {

      for (data::Sample::VI sample = samplesForOutput->begin(); sample != samplesForOutput->end(); sample++) {

	data::sample::Base::AP outputSample = data::sample::Base::AP(sampleCreator_());
	data::Sample::AP sample2(samplesForOutput->release(sample).release());
	outputSample->set(sample2);

	samples_->push_back(outputSample);
      }

      data::Samples samples;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "saveData()";

      // move all current samples to
      for (data::sample::Base::VI sample = samples_->begin(); sample != samples_->end();) {

	if (samples.getDate() != sample->getTime().date() && samples.size() != 0) {
	  samples.appendToFile(outputFile_);
	  samples.clear();
	}

	samples.add(samples_->release(sample));
      }

      if (samples.size() > 0) {
	samples.appendToFile(outputFile_);
      }
    }

  }
}
