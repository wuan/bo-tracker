#include <sys/socket.h>
#include <sys/un.h>

#include "data/Event.h"
#include "ipc/Listener.h"
#include "ipc/Server.h"

namespace blitzortung {
  namespace ipc {

    Listener::Listener(const unsigned int socket, sockaddr* sockaddr, socklen_t sockaddrSize) :
      socket_(socket),
      sockaddr_(sockaddr),
      sockaddrSize_(sockaddrSize),
      logger_("Listener")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "created for socket " << socket_;
    }

    void Listener::operator ()() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() running for socket " << socket_;

      for (;;) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "() listen";

        listen(socket_, 10);

	const unsigned int connectionSocket = accept(socket_, sockaddr_, &sockaddrSize_);

	if (connectionSocket == -1) {
	  logger_.errorStream() << "connection error";
	} else {
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() start server";

	  Server server(connectionSocket);
	  boost::thread thread(server);
	}

      }
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() done for socket " << socket_;
    }

  }
}
