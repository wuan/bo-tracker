#include <sys/socket.h>
#include <sys/un.h>

#include <cstdio>

#include <thread>

#include "ipc/Listener.h"
#include "ipc/UnixSocket.h"

namespace blitzortung {
  namespace ipc {

    UnixSocket::UnixSocket(const std::string& socketFileName, const ipc::server::factory::Base& serverFactory) :
      socket_(socket(AF_UNIX, SOCK_STREAM, 0)),
      socketFileName_(socketFileName),
      logger_("ipc.UnixSocket")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create unix domain socket " << socket_;

      sockaddr_un sockaddr;
      sockaddr.sun_family = AF_UNIX;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "bind to socket file '" << socketFileName_ << "'";

      strcpy(sockaddr.sun_path, socketFileName_.c_str());

      int failed = remove(sockaddr.sun_path);
      if (failed)
        logger_.warnStream() << "unlinking socket file '" << sockaddr.sun_path << "' failed";
	
      int addrlen = sizeof(sockaddr.sun_family) + strlen(sockaddr.sun_path); 

      failed = bind(socket_, (struct sockaddr*)&sockaddr, addrlen);

      if (!failed) {
        chmod(socketFileName_.c_str(), 0666);

        Listener listener(socket_, (struct sockaddr&)sockaddr, addrlen, serverFactory);

	logger_.debugStream() << "starting listener for socket " << socket_;

        std::thread(listener).detach();
      } else {
        logger_.warnStream() << "binding to socket file '" << sockaddr.sun_path << "' failed, socket-server not started";
      }
    }

    UnixSocket::~UnixSocket()
    {
      shutdown(socket_, SHUT_RDWR);

      remove(socketFileName_.c_str());

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "close socket " << socket_;
    }

  }
}
