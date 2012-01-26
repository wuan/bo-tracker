#ifndef BLITZORTUNG_IPC_SERVER_FACTORY_BASE_H_
#define BLITZORTUNG_IPC_SERVER_FACTORY_BASE_H_

#include <thread>

#include "Logger.h"
#include "ipc/server/Base.h"

namespace blitzortung {
  namespace ipc {
    namespace server {
      namespace factory {

	class Base {

	  private:

	    //! logger for this class
	    mutable Logger logger_;

	  public:

	    //! construct socket
	    Base();

	    virtual std::thread&& createServerThread(const unsigned int socket) const = 0; 
	};

      }
    }
  }
}

#endif
