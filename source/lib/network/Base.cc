#include "network/Base.h"
#include "exception/Base.h"
#include "network/transfer/Tcp.h"


namespace blitzortung {
  namespace network {

    Base::Base(const Creds& creds, const int sleepTime, const double eventRateLimit, const std::string& outputFile) :
      logger_("network.Base")
    {
      transfer::Tcp networkTransfer(sampleQueue_, creds, outputFile);

      networkTransfer.setSleepTime(sleepTime);
      networkTransfer.setEventRateLimit(eventRateLimit);

      boost::thread thread(networkTransfer);
    }

    Base::~Base() {
    }

    void Base::push(data::sample::Base::AP& data) {
      sampleQueue_.push(data);
    }

  }
}

