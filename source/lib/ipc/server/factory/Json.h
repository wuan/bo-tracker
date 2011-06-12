#ifndef BLITZORTUNG_IPC_SERVER_FACTORY_JSON_H_
#define BLITZORTUNG_IPC_SERVER_FACTORY_JSON_H_

#include "Logger.h"
#include "hardware/gps/Base.h"
#include "ipc/server/Json.h"
#include "ipc/server/factory/Base.h"

namespace blitzortung {
  namespace ipc {
    namespace server {
      namespace factory {

	class Json : public Base {

	  private:
	    
	    //! reference to process
	    const Process& process_;

	    //! reference to gps hardware
	    const hardware::Pcb& pcb_;

	    //! logger for this class
	    mutable Logger logger_;

	  public:

	    //! construct socket
	    Json(const Process& process, const hardware::Pcb& pcb);

	    //! create a server thread using an object of the corresponding class
	    void createServerThread(const unsigned int socket) const;
	};

      }
    }
  }
}

#endif
