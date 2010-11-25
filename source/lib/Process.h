#ifndef BLITZORTUNG_PROCESS_H_
#define BLITZORTUNG_PROCESS_H_

#include "namespaces.h"
#include "Logger.h"
#include "network/Creds.h"
#include "Queue.h"
#include "output/Base.h"
#include "network/transfer/Base.h"

#include "data/sample/Base.h"

namespace blitzortung {

  //! class providing data interface of network transfer
  class Process : private boost::noncopyable {

    public:

      typedef std::auto_ptr<Process> AP;

    private:
      //! queue for data transfer from measurement thread
      Queue<data::Sample> sampleQueue_;

      //! logger for this class
      mutable Logger logger_;

    public:
      //! constructor
      Process(network::transfer::Base& transfer, const pt::time_duration& sleepTime, const double eventRateLimit, output::Base& output);

      //! destructor
      virtual ~Process();

      //! add sample to send queue
      void push(data::Sample::AP&);
  };
}

#endif
