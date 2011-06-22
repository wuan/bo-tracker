#include "Process.h"
#include "DataThread.h"

#include "exception/Base.h"

namespace blitzortung {

  Process::Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output) :
    transfer_(transfer),
    output_(output),
    eventCountBuffer_(60*60),
    amplitudeLimit_(0.0),
    logger_("Process")
  {
    DataThread dataThread(eventQueue_, eventCountBuffer_, transfer, output);

    dataThread.setEventRateLimit(eventRateLimit);

    boost::thread thread(dataThread);
  }

  Process::~Process() {
  }

  void Process::push(data::Event::AP data) {
    if (logger_.isDebugEnabled())
      logger_.debugStream() << "push() " << data->getWaveform().getTime();

    const data::Waveform& waveform = data->getWaveform();
    float maximalSignalAmplitude = waveform.getAmplitude(waveform.getMaxIndex());
    if (waveform.isEmpty() || maximalSignalAmplitude >= amplitudeLimit_) {
      eventQueue_.push(data);
    } else {
      std::ostringstream oss;
      oss.precision(2);
      oss << "filtered signal with amplitude " << maximalSignalAmplitude << " at " << waveform.getTime().time_of_day();
      logger_.notice(oss.str());
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
}

