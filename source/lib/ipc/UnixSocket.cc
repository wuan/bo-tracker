#include <sys/socket.h>
#include <sys/un.h>

#include "ipc/Listener.h"
#include "ipc/UnixSocket.h"

namespace blitzortung {
  namespace ipc {

    UnixSocket::UnixSocket(const std::string& socketFileName) :
      logger_("UnixSocket")
    {
      socket_ = socket(AF_UNIX, SOCK_STREAM, 0);

      logger_.infoStream() << "create unix domain socket " << socket_;
      sockaddr_un sockaddr;
      sockaddr.sun_family = AF_UNIX;
      strcpy(sockaddr.sun_path, "/tmp/mysocket");
      unlink(sockaddr.sun_path);
      int addrlen = sizeof(sockaddr.sun_family) + strlen(sockaddr.sun_path); 
      bind(socket_, (struct sockaddr *)&sockaddr, addrlen);

      Listener listener(socket_, (struct sockaddr*)&sockaddr, addrlen);

      // start listener thread
      boost::thread thread(listener);
    }


    UnixSocket::~UnixSocket()
    {
      close(socket_);
      logger_.infoStream() << "close socket " << socket_;
    }

  }
}
