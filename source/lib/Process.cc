#include "Process.h"
#include "DataThread.h"

#include "exception/Base.h"

namespace blitzortung {

  Process::Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output) :
    startTime_(pt::second_clock::universal_time()),
    transfer_(transfer),
    output_(output),
    eventCountBuffer_(60*60),
    amplitudeLimit_(0.0),
    logger_("Process")
  {
    DataThread dataThread(eventQueue_, eventCountBuffer_, transfer, output);

    dataThread.setEventRateLimit(eventRateLimit);

    dataThread_ = std::move(std::thread(dataThread));
  }

  void Process::push(data::Event::AP&& data) {
    if (data.get() != 0) {
      const data::Waveform& waveform = data->getWaveform();

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "push() " << data->getWaveform().getTime();

      bool eventShouldBeProcessed = false;
      if (waveform.isEmpty()) {
	eventShouldBeProcessed = true;
      } else {
	float maximalSignalAmplitude = waveform.getAmplitude(waveform.getMaxIndex());
	if (maximalSignalAmplitude >= amplitudeLimit_) {
	  eventShouldBeProcessed = true;
	} else {
	  std::ostringstream oss;
	  oss.precision(2);
	  oss << "filtered signal with amplitude " << maximalSignalAmplitude << " at " << waveform.getTime().time_of_day();
	  logger_.notice(oss.str());
	}
      }
      if (eventShouldBeProcessed)
	eventQueue_.push(std::move(data));
    } else {
      logger_.warnStream() << "received empty event";
    }
  }

  const DataThread::EventCountBuffer& Process::getEventCountBuffer() const {
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

