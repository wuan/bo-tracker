#include "ipc/server/factory/Json.h"

namespace blitzortung {
  namespace ipc {
    namespace server {
      namespace factory {

	Json::Json(const Process& process, const hardware::Pcb& pcb) :
	  process_(process),
	  pcb_(pcb),
	  logger_("іpc.server.factory.Json")
	{
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "initialize factory ";
	}

	void Json::createServerThread(unsigned int socket) const {
	  boost::thread(ipc::server::Json(socket, process_, pcb_));
	}

      }
    }
  }
}
