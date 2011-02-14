#ifndef BLITZORTUNG_DATATHREAD_H_
#define BLITZORTUNG_DATATHREAD_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/condition.hpp>

#include "data/Events.h"
#include "Queue.h"
#include "network/transfer/Base.h"
#include "output/Base.h"
#include "Logger.h"

namespace blitzortung {

  class DataThread {

    private:
      //! time between successive tranfers in seconds
      pt::time_duration sleepTime_;

      //! reference to incoming sample data queue
      Queue<bo::data::Event>& sampleQueue_;

      //! network data transfer object
      const network::transfer::Base& transfer_;

      //! vector of samples
      data::Events::P events_;

      //! limit value of maximum events / second;
      double eventRateLimit_;

      //! output object
      output::Base& output_;

      //! logger for this class
      mutable Logger logger_;

      //! prepare data for transmission
      data::Events::AP prepareData(const pt::ptime&, const pt::ptime&);

      //! get string to be transmitted for every sample
      std::string sampleToString(const data::Event& sample);

    public:

      //! create network transfer object
      DataThread(Queue<data::Event>& sampleQueue, const network::transfer::Base& transfer, output::Base&);

      //! delete nework transfer object
      virtual ~DataThread();

      //! set sleep time between data transfers in seconds
      void setSleepTime(const pt::time_duration& sleepTime);

      //! set limit of average number of events per minute transmitted
      void setEventRateLimit(const double eventRateLimit);

      //! network transfer thread: wait for new samples and send them from time to time
      void operator ()();

  };
}

#endif
