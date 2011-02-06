#include "network/transfer/None.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      None::None(const Creds& creds) :
	Base(creds), 
	logger_("network.transfer.None")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      None::~None() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "destroyed";

      }

      std::string None::eventToString(blitzortung::data::Event const& event) const {
	std::ostringstream oss;

	oss << event;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "eventToString() : " << oss.str();

	return oss.str();
      }


      void None::send(const data::Event::VP& events) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "send() : " << events->size() << "events";
      }

    }
  }
}
