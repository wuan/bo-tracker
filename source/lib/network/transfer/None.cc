#include "network/transfer/None.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      None::None() :
	Base(localCreds_),
	logger_("network.transfer.None")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      None::~None() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "destroyed";

      }

      void None::send(const data::Events& events) const {
	if (logger_.isDebugEnabled()) {
	  logger_.debugStream() << "send() : " << events.size() << "events";

	  for (data::Event::CVI event = events.begin(); event != events.end(); event++) {
	    logger_.debugStream() << eventToString(*event);
	  }
	}
      }

    }
  }
}
