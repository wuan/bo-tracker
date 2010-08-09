#ifndef BLITZORTUNG_NETWORK_BASE_H_
#define BLITZORTUNG_NETWORK_BASE_H_


#include "namespaces.h"
#include "network/Creds.h"
#include "network/Queue.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace network {

    class Base : private boost::noncopyable {

      private:
	Queue<bo::data::sample::Base> sampleQueue_;

      public:
	Base(const Creds& creds, const int sleepTime, const double eventRateLimit);

	virtual ~Base();

	//! add sample to send queue
	void put(std::auto_ptr<data::sample::Base>);
    };
  }
}

#endif
