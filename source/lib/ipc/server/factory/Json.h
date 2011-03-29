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
	    
	    //! reference to gps hardware
	    const hardware::gps::Base& gps_;

	    //! logger for this class
	    mutable Logger logger_;

	  public:

	    //! construct socket
	    Json(const hardware::gps::Base& gps);

	    boost::thread createServerThread(const unsigned int socket) const;
	};

      }
    }
  }
}

#endif
