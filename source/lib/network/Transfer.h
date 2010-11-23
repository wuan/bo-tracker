#ifndef BLITZORTUNG_NETWORK_TRANSFER_H_
#define BLITZORTUNG_NETWORK_TRANSFER_H_

#include <arpa/inet.h>
#include <netdb.h>

#include <queue>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/condition.hpp>

#include "data/Samples.h"
#include "network/Base.h"
#include "network/Queue.h"
#include "network/Creds.h"
#include "output/File.h"
#include "Logger.h"

namespace blitzortung {
  namespace network {

    class Transfer {

      private:
	//! time between successive tranfers in seconds
	int sleepTime_;

	//! reference to incoming sample data queue
	Queue<bo::data::Sample>& sampleQueue_;

	//! credential and host information for network data transfer
	const Creds& creds_;

	//! vector of samples
	data::Sample::VP samples_;

	//! limit value of maximum events / second;
	double eventRateLimit_;

	//! output file name
	output::Base& output_;
	
	//! logger for this class
	mutable Logger logger_;

	//! prepare data for transmission
	data::Sample::VP prepareData(pt::ptime&, pt::ptime&);
	
	//! send data to server
	void sendData();
	
	//! get string to be transmitted for every sample
	std::string sampleToString(const data::Sample& sample);
	
      public:

	//! create network transfer object
	Transfer(Queue<data::Sample>& sampleQueue, const Creds& creds, output::Base&);

	//! delete nework transfer object
	virtual ~Transfer();

	//! set sleep time between data transfers in seconds
	void setSleepTime(const int sleepTime);

	//! set limit of average number of events per minute transmitted
	void setEventRateLimit(const double eventRateLimit);

	//! network transfer thread: wait for new samples and send them from time to time
	void operator ()();

    };
  }
}

#endif
