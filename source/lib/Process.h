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
  class Process {

    public:

      typedef std::auto_ptr<Process> AP;

    private:

      //! time when process object was created
      const pt::ptime startTime_;

      //! queue for data transfer from measurement thread
      Queue<data::Event> eventQueue_;

      //! reference to transfer object
      const network::transfer::Base& transfer_;
      
      //! reference to output object
      const output::Base& output_;

      //! ring buffer for data rate limiter
      DataThread::EventCountBuffer eventCountBuffer_;

      //! data thread
      std::thread dataThread_;

      //! lower limit of amplitude
      double amplitudeLimit_;
      
      //! logger for this class
      mutable Logger logger_;

      // disable copying of objects
      Process(const Process&) = delete;
      Process& operator=(const Process&) = delete;

    public:
      //! constructor
      Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output);

      //! destructor
      virtual ~Process() = default;

      //! add event to send queue
      void push(data::Event::AP&&);

      //! get reference to event count buffer
      const DataThread::EventCountBuffer& getEventCountBuffer() const;
      
      //! get reference to transfer object
      const network::transfer::Base& getTransfer() const;
      
      //! get reference to output object
      const output::Base& getOutput() const;

      //! set amplitude limit
      void setAmplitudeLimit(double amplitudeLimit);

      //! get start time of process object
      const pt::ptime& getStartTime() const;

      //! get uptime of process object
      const pt::time_duration getUptime() const;
  };
}

#endif
