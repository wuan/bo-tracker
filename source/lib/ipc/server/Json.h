#ifndef BLITZORTUNG_IPC_SERVER_JSON_H_
#define BLITZORTUNG_IPC_SERVER_JSON_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "hardware/gps/Base.h"
#include "ipc/server/Base.h"
#include "Process.h"
#include "Logger.h"

namespace blitzortung {
  namespace ipc {
    namespace server {

      class Json : public Base {

	private:

	  //! process reference
	  const Process& process_;

	  //! gps hardware reference
	  const hardware::gps::Base& gps_;

	  //! logger for this class
	  mutable Logger logger_;

	  virtual std::string respond(const std::string&);

	public:

	  Json(const unsigned int socket, const Process& process, const hardware::gps::Base& gps);

      };

    }
  }
}

#endif
