#ifndef BLITZORTUNG_IPC_LISTENER_H_
#define BLITZORTUNG_IPC_LISTENER_H_

#include <sys/socket.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "Logger.h"

namespace blitzortung {
  namespace ipc {

    class Listener {

      private:

	//! socket for which this listener is active
	const unsigned int socket_;

	sockaddr* sockaddr_;

	socklen_t sockaddrSize_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	Listener(const unsigned int socket, sockaddr* sockaddr, socklen_t sockaddrSize);

	//! listener thread procedure
	void operator ()();
    };

  }
}

#endif
