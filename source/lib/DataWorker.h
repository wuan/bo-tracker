#ifndef BLITZORTUNG_DATATHREAD_H_
#define BLITZORTUNG_DATATHREAD_H_

#include "data/Events.h"
#include "Queue.h"
#include "network/transfer/Base.h"
#include "output/Base.h"
#include "Logger.h"
#include "util/RingBuffer.h"

namespace blitzortung {

  class DataWorker {

    public:

      typedef util::RingBuffer<unsigned short> EventCountBuffer;

    private:
      //! reference to incoming sample data queue
      Queue<bo::data::Event>& sampleQueue_;

      //! ring buffer for rate limiter
      EventCountBuffer& eventCountBuffer_;

      //! network data transfer object
      network::transfer::Base& transfer_;

      //! vector of samples
      data::Events::P events_;

      //! output object
      output::Base& output_;

      //! limit value of maximum events / second;
      double eventRateLimit_;

      //! logger for this class
      mutable Logger logger_;

      //! prepare data for transmission
      data::Events::AP prepareData(int eventsPerSecond);

      //! get string to be transmitted for every sample
      std::string sampleToString(const data::Event& sample);

    public:

      //! create network transfer object
      DataWorker(Queue<data::Event>& sampleQueue, EventCountBuffer& eventCountBuffer, network::transfer::Base& transfer, output::Base&);

      //! set limit of average number of events per minute transmitted
      void setEventRateLimit(const double eventRateLimit);

      //! network transfer thread: wait for new samples and send them from time to time
      void operator ()();

      pt::ptime getSecond() const;
  };
}

#endif
