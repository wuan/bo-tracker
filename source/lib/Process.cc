#include "Process.h"
#include "DataThread.h"

#include "exception/Base.h"

namespace blitzortung {

  Process::Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output) :
    logger_("Process")
  {
    DataThread dataThread(eventQueue_, transfer, output);

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

}

