#include <sys/socket.h>
#include <sys/un.h>

#include "Unix.h"
#include "Listener.h"

namespace blitzortung {

  Unix::Unix(const std::string& socketFileName) :
    logger_("Unix")
  {
    struct sockaddr_un sockaddr;
    int len;

    socket_ = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr.sun_family = AF_UNIX;  /* local is declared before socket() ^ */
    strcpy(sockaddr.sun_path, "/tmp/mysocket");
    unlink(sockaddr.sun_path);
    len = strlen(sockaddr.sun_path) + sizeof(sockaddr.sun_family);
    bind(socket_, (struct sockaddr *)&sockaddr, len);

    Listener listener(socket_);

    // start listener thread
    boost::thread thread(listener);
  }


  Unix::~Unix()
  {
    // TODO add code to free content of queue here
  }

}
