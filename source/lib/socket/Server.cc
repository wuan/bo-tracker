#include <sys/socket.h>
#include <sys/un.h>

#include "data/Event.h"
#include "Server.h"

namespace blitzortung {

  Server::Server(const unsigned int socket) :
    logger_("Server")
  {
    logger_.infoStream() << "initialize server for socket " << socket;
    size_t sockaddrSize = sizeof(sockaddr_);
    socket_ = accept(socket, (sockaddr*)&sockaddr_, &sockaddrSize);
    logger_.infoStream() << "initialize new socket " << socket_;
  }

  Server::~Server()
  {
  }

  void Server::operator ()() {
    logger_.infoStream() << "() started";

    char buffer[100];
    bool done = false;

    do {
      int n = recv(socket_, buffer, 100, 0);
      if (n <= 0) {
	done = true;
      }
      logger_.infoStream() << "() read: '" << buffer << "'";

      if (!done) {
	if (send(socket_, buffer, n, 0) < 0) {
	  done = true;
	}
      }
    } while(!done);

    close(socket_);
  }

}
