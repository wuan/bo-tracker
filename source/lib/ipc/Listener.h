#ifndef BLITZORTUNG_IPC_LISTENER_H_
#define BLITZORTUNG_IPC_LISTENER_H_

#include <sys/socket.h>

#include "Logger.h"
#include "ipc/server/factory/Base.h"

namespace blitzortung {
  namespace ipc {

    class Listener {

      private:

	//! socket for which this listener is active
	const unsigned int socket_;

	sockaddr& sockaddr_;

	socklen_t sockaddrSize_;
	
	// reference to server factory object	
	const ipc::server::factory::Base& serverFactory_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	Listener(const unsigned int socket, sockaddr& sockaddr, socklen_t sockaddrSize, const ipc::server::factory::Base& serverFactory);

	//! listener thread procedure
	void operator ()();
    };

  }
}

#endif
