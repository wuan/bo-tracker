#ifndef BLITZORTUNG_NETWORK_TRANSFER_TCP_H_
#define BLITZORTUNG_NETWORK_TRANSFER_TCP_H_

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
#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class Tcp : public Base {

	private:
	  //! logger for this class
	  mutable Logger logger_;

	  //! initialize network connection to server
	  int openConnection ();

	public:

	  //! create network transfer object
	  Tcp(Queue<data::sample::Base>& sampleQueue, const Creds& creds, const std::string&);

	  //! delete nework transfer object
	  virtual ~Tcp();

	  //! network transfer thread: wait for new samples and send them from time to time
	  void operator ()();

      };

    }
  }
}

#endif
