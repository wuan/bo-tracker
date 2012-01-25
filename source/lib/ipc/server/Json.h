#ifndef BLITZORTUNG_IPC_SERVER_JSON_H_
#define BLITZORTUNG_IPC_SERVER_JSON_H_

#include <json/json.h>

#include "hardware/Pcb.h"
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
	  const hardware::Pcb& hardware_;

	  json_object* jsonResponse_;

	  //! logger for this class
	  mutable Logger logger_;

	  void cmdGetInfo();

	  void cmdGetActivity();

	  virtual std::string respond(const std::string&);

	public:

	  Json(const unsigned int socket, const Process& process, const hardware::Pcb& hardware);

      };

    }
  }
}

#endif
