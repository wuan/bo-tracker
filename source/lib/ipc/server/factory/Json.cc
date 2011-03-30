#include "ipc/server/factory/Json.h"

namespace blitzortung {
  namespace ipc {
    namespace server {
      namespace factory {

	Json::Json(const Process& process, const hardware::gps::Base& gps) :
	  process_(process),
	  gps_(gps),
	  logger_("іpc.server.factory.Json")
	{
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "initialize factory ";
	}

	boost::thread Json::createServerThread(unsigned int socket) const {
	  return boost::thread(ipc::server::Json(socket, process_, gps_));
	}

      }
    }
  }
}
