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
      logger_.infoStream() << "created for socket " << socket_;
    }

    void Listener::operator ()() {
      logger_.infoStream() << "() running for socket " << socket_;
      for (;;) {
	logger_.infoStream() << "() listen";
        listen(socket_, 10);

	int connectionSocket = accept(socket_, sockaddr_, &sockaddrSize_);
	if (connectionSocket == -1) {
	  logger_.errorStream() << "connection error";
	} else {
	  logger_.infoStream() << "() start server";
	  Server server(connectionSocket);

	  boost::thread thread(server);
	}

      }
      logger_.infoStream() << "() done for socket " << socket_;
    }

  }
}
