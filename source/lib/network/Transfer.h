#ifndef BLITZORTUNG_NETWORK_TRANSFER_H_
#define BLITZORTUNG_NETWORK_TRANSFER_H_

#include <arpa/inet.h>
#include <netdb.h>

#include <queue>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/condition.hpp>

#include "data/sample/Base.h"
#include "network/Queue.h"
#include "network/Creds.h"

namespace blitzortung {
  namespace network {

    class Transfer {
      private:
	int sleepTime_;
	Queue<data::sample::Base>& sampleQueue_;
	const Creds& creds_;
	data::sample::Base::VP samples_;
	double eventRateLimit_;

      public:
	Transfer(Queue<bo::data::sample::Base>& sampleQueue, const Creds& creds);

	virtual ~Transfer();

	//! set sleep time between data transfers in seconds
	void setSleepTime(const int sleepTime);

	//! set limit of average number of events per minute transmitted
	void setEventRateLimit(const double eventRateLimit);

	//! initialize network connection to server
	int open_connection ();

	//! get string to be transmitted for every sample
	std::string sampleToString(const data::sample::Base& sample);

	//! network transfer thread: wait for new samples and send them from time to time
	void operator ()();
    };
  }
}

#endif
