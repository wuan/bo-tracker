#ifndef BLITZORTUNG_IPC_SERVER_H_
#define BLITZORTUNG_IPC_SERVER_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "Logger.h"

namespace blitzortung {
  namespace ipc {

    class Server {

      private:

	sockaddr_un sockaddr_;

	unsigned int socket_;

	//! logger for this class
	mutable Logger logger_;

	std::string respond(const std::string&);

      public:

	//! construct socket
	Server(const unsigned int socket);

	//! server thread procedure
	void operator ()();

    };

  }
}

#endif
