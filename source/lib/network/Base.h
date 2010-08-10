#ifndef BLITZORTUNG_NETWORK_BASE_H_
#define BLITZORTUNG_NETWORK_BASE_H_


#include "namespaces.h"
#include "network/Creds.h"
#include "network/Queue.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace network {

    //! class providing data interface of network transfer
    class Base : private boost::noncopyable {

      private:
	//! queue for data transfer from measurement thread
	Queue<bo::data::sample::Base> sampleQueue_;

      public:
	//! constructor
	Base(const Creds& creds, const int sleepTime, const double eventRateLimit);

	//! destructor
	virtual ~Base();

	//! add sample to send queue
	void put(std::auto_ptr<data::sample::Base>);
    };
  }
}

#endif
