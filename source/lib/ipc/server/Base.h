#ifndef BLITZORTUNG_IPC_SERVER_BASE_H_
#define BLITZORTUNG_IPC_SERVER_BASE_H_

#include <sys/socket.h>
#include <sys/un.h>

#include "Logger.h"

namespace blitzortung {
  namespace ipc {
    namespace server {

      class Base {
		
        public:
	  typedef std::auto_ptr<Base> AP;

	private:

	  sockaddr_un sockaddr_;

	  unsigned int socket_;

	  //! logger for this class
	  mutable Logger logger_;

	  virtual std::string respond(const std::string&);

	public:

	  //! construct socket
	  Base(const unsigned int socket);

	  //! virtual destructor
	  virtual ~Base() {};

	  //! server thread procedure
	  void operator ()();

      };

    }
  }
}

#endif
