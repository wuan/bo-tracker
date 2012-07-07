#include <sys/socket.h>
#include <sys/un.h>

#include "data/Event.h"
#include "ipc/Listener.h"
#include "ipc/server/factory/Base.h"

namespace blitzortung {
  namespace ipc {

    Listener::Listener(const unsigned int socket, sockaddr& sockaddr, socklen_t sockaddrSize, const ipc::server::factory::Base& serverFactory) :
      socket_(socket),
      sockaddr_(sockaddr),
      sockaddrSize_(sockaddrSize),
      serverFactory_(serverFactory),
      logger_("ipc.Listener")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "created for socket " << socket_;
    }

    void Listener::operator ()() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() running for socket " << socket_;

      for (;;) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "() wait for new connection";

        listen(socket_, 10);

	const int connectionSocket = accept(socket_, &sockaddr_, &sockaddrSize_);

	if (connectionSocket == -1) {
	  logger_.errorStream() << "connection error";
	} else {
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() start server";

	  serverFactory_.createServerThread(connectionSocket);
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "  received server thread";
	}

      }
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() done for socket " << socket_;
    }

  }
}
