#include "Process.h"
#include "DataWorker.h"

#include "exception/Base.h"

namespace blitzortung {

  Process::Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output) :
    startTime_(pt::second_clock::universal_time()),
    eventQueue_(),
    transfer_(transfer),
    output_(output),
    eventCountBuffer_(60*60),
    amplitudeLimit_(0.0),
    logger_("Process")
  {
    DataWorker dataWorker(eventQueue_, eventCountBuffer_, transfer, output);

    dataWorker.setEventRateLimit(eventRateLimit);

    std::thread dataThread(dataWorker);
    dataThread.detach();
  }

  Process::~Process()
  {
  }

  void Process::push(data::Event::AP&& data) {
    if (data.get() != 0) {
      const data::Waveform& waveform = data->getWaveform();

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "push() " << data->getWaveform().getTimestamp();

      bool eventShouldBeProcessed = false;
      if (waveform.isEmpty()) {
	eventShouldBeProcessed = true;
      } else {
	float maximalSignalAmplitude = waveform.getAmplitude(waveform.getMaxIndex());
	if (maximalSignalAmplitude >= amplitudeLimit_) {
	  eventShouldBeProcessed = true;
	} else {
	  if (logger_.isInfoEnabled()) {
	    std::ostringstream oss;
	    oss.precision(2);
	    oss << "filtered signal with amplitude " << maximalSignalAmplitude << " at " << waveform.getTimestamp().time_of_day();
	    logger_.info(oss.str());
	  }
	}
      }
      if (eventShouldBeProcessed)
	eventQueue_.push(std::move(data));
    } else {
      logger_.warnStream() << "received empty event";
    }
  }

  const DataWorker::EventCountBuffer& Process::getEventCountBuffer() const {
    return eventCountBuffer_;
  }
  
  const network::transfer::Base& Process::getTransfer() const {
    return transfer_;
  }
      
  const output::Base& Process::getOutput() const {
    return output_;
  }

  void Process::setAmplitudeLimit(double amplitudeLimit) {
    amplitudeLimit_ = amplitudeLimit;
  }

  const pt::ptime& Process::getStartTime() const {
    return startTime_;
  }

  const pt::time_duration Process::getUptime() const {
    return pt::second_clock::universal_time() - getStartTime();
  }
}

