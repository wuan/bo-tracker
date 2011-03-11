#include <sys/socket.h>
#include <sys/un.h>

#include "data/Event.h"
#include "Listener.h"

namespace blitzortung {

  Listener::Listener(const unsigned int socket) :
    socket_(socket),
    logger_("Listener")
  {
  }


  Listener::~Listener()
  {
    // TODO add code to free content of queue here
  }

  void Listener::operator ()() {
    listen(socket_, 10);
  }

}
