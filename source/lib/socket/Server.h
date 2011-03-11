#ifndef BLITZORTUNG_SOCKET_LISTENER_H_
#define BLITZORTUNG_SOCKET_LISTENER_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "Logger.h"

namespace blitzortung {

    class Server {

      private:

	sockaddr_un sockaddr_;
	unsigned int socket_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	Server(const unsigned int socket);

	//! destruct socket
	virtual ~Server();

	//! server thread procedure
	void operator ()();
    };

}

#endif
