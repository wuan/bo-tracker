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

      std::string None::sampleToString(blitzortung::data::Sample const& sample) const {
	std::ostringstream oss;

	oss << sample;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "sampleToString() : " << oss.str();

	return oss.str();
      }


      void None::send(const data::Sample::VP& samples) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "send() : " << samples->size() << "samples";
      }

    }
  }
}
