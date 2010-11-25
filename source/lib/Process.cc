#include "Process.h"
#include "DataThread.h"

#include "exception/Base.h"

namespace blitzortung {

  Process::Process(network::transfer::Base& transfer, const pt::time_duration& sleepTime, const double eventRateLimit, output::Base& output) :
    logger_("Process")
  {
    DataThread dataThread(sampleQueue_, transfer, output);

    dataThread.setSleepTime(sleepTime);
    dataThread.setEventRateLimit(eventRateLimit);

    boost::thread thread(dataThread);
  }

  Process::~Process() {
  }

  void Process::push(data::Sample::AP& data) {
    sampleQueue_.push(data);
  }

}

