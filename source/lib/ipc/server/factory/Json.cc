#include "ipc/server/factory/Json.h"

namespace blitzortung {
  namespace ipc {
    namespace server {
      namespace factory {

	Json::Json(const hardware::gps::Base& gps) :
	  gps_(gps),
	  logger_("іpc.server.factory.Json")
	{
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "initialize factory ";
	}

	boost::thread Json::createServerThread(unsigned int socket) const {
	  return boost::thread(ipc::server::Json(socket, gps_));
	}

      }
    }
  }
}
