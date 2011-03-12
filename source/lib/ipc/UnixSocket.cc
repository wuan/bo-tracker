#include <sys/socket.h>
#include <sys/un.h>

#include "ipc/Listener.h"
#include "ipc/UnixSocket.h"

namespace blitzortung {
  namespace ipc {

    UnixSocket::UnixSocket(const std::string& socketFileName) :
      socketFileName_(socketFileName), 
      logger_("UnixSocket")
    {
      socket_ = socket(AF_UNIX, SOCK_STREAM, 0);

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create unix domain socket " << socket_;

      sockaddr_un sockaddr;
      sockaddr.sun_family = AF_UNIX;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "bind to socket file '" << socketFileName << "'";

      strcpy(sockaddr.sun_path, socketFileName.c_str());

      unlink(sockaddr.sun_path);
      int addrlen = sizeof(sockaddr.sun_family) + strlen(sockaddr.sun_path); 

      bind(socket_, (struct sockaddr *)&sockaddr, addrlen);

      Listener listener(socket_, (struct sockaddr*)&sockaddr, addrlen);

      boost::thread thread(listener);
    }


    UnixSocket::~UnixSocket()
    {
      close(socket_);
      unlink(socketFileName_.c_str());
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "close socket " << socket_;
    }

  }
}
