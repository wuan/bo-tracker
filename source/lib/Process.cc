#include "Process.h"
#include "DataThread.h"

#include "exception/Base.h"

namespace blitzortung {

  Process::Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output) :
    eventCountBuffer_(60*60),
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
    eventQueue_.push(data);
  }

  const DataThread::EventCountBuffer& Process::getEventCountBuffer() const {
    return eventCountBuffer_;
  }
}

