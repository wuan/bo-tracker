#ifndef BLITZORTUNG_PROCESS_H_
#define BLITZORTUNG_PROCESS_H_

#include "namespaces.h"
#include "Logger.h"
#include "network/Creds.h"
#include "Queue.h"
#include "output/Base.h"
#include "network/transfer/Base.h"

#include "data/Event.h"

namespace blitzortung {

  //! class providing data interface of network transfer
  class Process : private boost::noncopyable {

    public:

      typedef std::auto_ptr<Process> AP;

    private:
      //! queue for data transfer from measurement thread
      Queue<data::Event> eventQueue_;

      //! logger for this class
      mutable Logger logger_;

    public:
      //! constructor
      Process(network::transfer::Base& transfer, const double eventRateLimit, output::Base& output);

      //! destructor
      virtual ~Process();

      //! add event to send queue
      void push(data::Event::AP);
  };
}

#endif
