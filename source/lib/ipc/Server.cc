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
      if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialize server for socket " << socket_;
    }

    void Server::operator ()() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() started for socket " << socket_;

      char buffer[100];
      bool done = false;

      do {
	int n = recv(socket_, buffer, 100, 0);
	if (n <= 0) {
	  done = true;
	  if (n==0) {
	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() connection closed";
	  } else
	    logger_.warnStream() << "() recv failed " << n;
	} else {
	  buffer[n]=0;
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() read: '" << buffer << "' " << n;

	  std::string input(buffer);
	  std::string output = respond(input);

	  if (!done && output.size() > 0) {
	    n = send(socket_, output.c_str(), output.size(), 0);
	    if (n < 0) {
	      done = true;
	      logger_.warnStream() << "() send failed " << n;
	    }
	  }
	}
      } while (!done);

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() ended";
      close(socket_);
    }

    std::string Server::respond(const std::string& input) {
      return input;
    }

  }
}