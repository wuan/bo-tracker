#ifndef BLITZORTUNG_NETWORK_BASE_H_
#define BLITZORTUNG_NETWORK_BASE_H_


#include "namespaces.h"
#include "Logger.h"
#include "network/Creds.h"
#include "network/Queue.h"
#include "output/File.h"
#include "output/None.h"

#include "data/sample/Base.h"

namespace blitzortung {
  namespace network {

    //! class providing data interface of network transfer
    class Base : private boost::noncopyable {

      public:

	  typedef std::auto_ptr<Base> AP;

      private:
	//! queue for data transfer from measurement thread
	Queue<data::Sample> sampleQueue_;

	//! logger for this class
	mutable Logger logger_;

      public:
	//! constructor
	Base(const Creds& creds, const int sleepTime, const double eventRateLimit, output::Base& output);

	//! destructor
	virtual ~Base();

	//! add sample to send queue
	void push(data::Sample::AP&);
    };
  }
}

#endif
