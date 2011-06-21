#ifndef BLITZORTUNG_PROCESS_H_
#define BLITZORTUNG_PROCESS_H_

#include "namespaces.h"
#include "Logger.h"
#include "network/Creds.h"
#include "Queue.h"
#include "output/Base.h"
#include "network/transfer/Base.h"
#include "DataThread.h"

#include "data/Event.h"

namespace blitzortung {

  //! class providing data interface of network transfer
  class Process : private boost::noncopyable {

    public:

      typedef std::auto_ptr<Process> AP;

    private:
      //! queue for data transfer from measurement thread
      Queue<data::Event> eventQueue_;

      //! reference to transfer object
      const network::transfer::Base& transfer_;
      
      //! reference to output object
      const output::Base& output_;

      //! ring buffer for data rate limiter
      DataThread::EventCountBuffer eventCountBuffer_;

      //! lower limit of amplitude
      double amplitudeLimit_;
      
      //! logger for this class
      mutable Logger logger_;

    public:
      //! constructor
      Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output);

      //! destructor
      virtual ~Process();

      //! add event to send queue
      void push(data::Event::AP);

      //! get reference to event count buffer
      const DataThread::EventCountBuffer& getEventCountBuffer() const;
      
      //! get reference to transfer object
      const network::transfer::Base& getTransfer() const;
      
      //! get reference to output object
      const output::Base& getOutput() const;

      //! set amplitude limit
      void setAmplitudeLimit(double amplitudeLimit);

  };
}

#endif
