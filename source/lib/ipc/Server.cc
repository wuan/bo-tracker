#include <sys/socket.h>
#include <sys/un.h>

#include "data/Event.h"
#include "Server.h"

namespace blitzortung {
  namespace ipc {

    Server::Server(const unsigned int socket) :
      socket_(socket),
      logger_("Server")
    {
      logger_.infoStream() << "initialize server for socket " << socket_;
    }

    void Server::operator ()() {
      logger_.infoStream() << "() started for socket " << socket_;

      char buffer[100];
      bool done = false;

      do {
	int n = recv(socket_, buffer, 100, 0);
	if (n <= 0) {
	  done = true;
	  if (n==0)
	    logger_.infoStream() << "() connection closed";
	  else
	    logger_.warnStream() << "() recv failed " << n;
	} else {
	  buffer[n]=0;
	  logger_.infoStream() << "() read: '" << buffer << "' " << n;

	  if (!done) {
	    if (send(socket_, buffer, n, 0) < 0) {
	      done = true;
	      logger_.warnStream() << "() send failed " << n;
	    }
	  }
	}
      } while(!done);

      logger_.infoStream() << "() ended";
      close(socket_);
    }

  }
}
