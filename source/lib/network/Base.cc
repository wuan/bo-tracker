#include "network/Base.h"
#include "exception/Base.h"
#include "network/Transfer.h"


namespace blitzortung {
  namespace network {

    Base::Base(const Creds& creds, const int sleepTime, const double eventRateLimit, output::Base& output) :
      logger_("network.Base")
    {
      Transfer networkTransfer(sampleQueue_, creds, output);

      networkTransfer.setSleepTime(sleepTime);
      networkTransfer.setEventRateLimit(eventRateLimit);

      boost::thread thread(networkTransfer);
    }

    Base::~Base() {
    }

    void Base::push(data::Sample::AP& data) {
      sampleQueue_.push(data);
    }

  }
}

