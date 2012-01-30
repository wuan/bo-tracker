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
	  std::thread thread(ipc::server::Json(socket, process_, pcb_));

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "create server thread for socket " << socket;

	  thread.detach();
	}

      }
    }
  }
}
