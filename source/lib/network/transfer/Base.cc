#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      Base::Base(const Creds& creds) :
	logger_("network.transfer.Base"),
	creds_(creds)
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      Base::~Base() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "destroyed";

      }
    }
  }
}
