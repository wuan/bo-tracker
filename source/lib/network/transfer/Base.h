#ifndef BLITZORTUNG_NETWORK_TRANSFER_BASE_H_
#define BLITZORTUNG_NETWORK_TRANSFER_BASE_H_

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
#include "Logger.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class Base {

	protected:
	  //! time between successive tranfers in seconds
	  int sleepTime_;

	  //! reference to incoming sample data queue
	  Queue<bo::data::sample::Base>& sampleQueue_;

	  //! credential and host information for network data transfer
	  const Creds& creds_;

	  //! vector of samples
	  data::sample::Base::VP samples_;

	  //! limit value of maximum events / second;
	  double eventRateLimit_;

	  //! output file name
	  const std::string& outputFile_;

	  //! get string to be transmitted for every sample
	  std::string sampleToString(const data::sample::Base& sample);

	  //! save data to file
	  void saveData();

	private:
	  //! logger for this class
	  mutable Logger logger_;

	public:

	  //! create network transfer object
	  Base(Queue<data::sample::Base>& sampleQueue, const Creds& creds, const std::string&);

	  //! delete nework transfer object
	  virtual ~Base();

	  //! set sleep time between data transfers in seconds
	  void setSleepTime(const int sleepTime);

	  //! set limit of average number of events per minute transmitted
	  void setEventRateLimit(const double eventRateLimit);

	  //! network transfer thread: wait for new samples and send them from time to time
	  virtual void operator ()() = 0;

      };

    }
  }
}

#endif
