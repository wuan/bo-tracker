#include <sys/socket.h>
#include <sys/un.h>

#include "ipc/Listener.h"
#include "ipc/UnixSocket.h"

namespace blitzortung {
  namespace ipc {

    UnixSocket::UnixSocket(const std::string& socketFileName, const ipc::server::factory::Base& serverFactory) :
      socketFileName_(socketFileName),
      logger_("ipc.UnixSocket")
    {
      socket_ = socket(AF_UNIX, SOCK_STREAM, 0);

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create unix domain socket " << socket_;

      sockaddr_un sockaddr;
      sockaddr.sun_family = AF_UNIX;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "bind to socket file '" << socketFileName_ << "'";

      strcpy(sockaddr.sun_path, socketFileName_.c_str());

      int success = unlink(sockaddr.sun_path);
      if (!success)
        logger_.warnStream() << "unlinking socket file '" << sockaddr.sun_path << "' failed";
	
      int addrlen = sizeof(sockaddr.sun_family) + strlen(sockaddr.sun_path); 

      success = bind(socket_, (struct sockaddr *)&sockaddr, addrlen);
      if (success) {
        chmod(socketFileName_.c_str(), 0666);

        Listener listener(socket_, (struct sockaddr*)&sockaddr, addrlen, serverFactory);

        boost::thread thread(listener);
      } else {
        logger_.warnStream() << "binding to socket file '" << sockaddr.sun_path << "' failed, socket-server not started";
      }

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
