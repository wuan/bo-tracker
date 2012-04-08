#include "network/transfer/Base.h"

#include "data/MEvent.h"

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

      std::string Base::eventToString(const data::Event& event) const {
	const data::Waveform& wfm = event.getWaveform();
	const data::GpsInfo& gpsInfo = event.getGpsInfo();

	std::ostringstream oss;

	pt::time_facet *timefacet = new pt::time_facet();
	timefacet->format("%Y-%m-%d %H:%M:%S.%f");
	std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

	oss << wfm.getTime();
	oss.setf(std::ios::fixed);
	oss.precision(6);

	oss << " " << gpsInfo.getLatitude() << " " << gpsInfo.getLongitude();
	oss << " " << gpsInfo.getAltitude();
	oss << " " << creds_.getUsername() << " " << creds_.getPassword();
	oss << " " << gpsInfo.getStatus() << " " << gpsInfo.getNumberOfSatellites();
	if (wfm.isEmpty()) {
	  oss << " 0 0 0";
	} else {
	  oss << " " << static_cast<const data::MEvent&>(event).getRawData();
	}

	// restore original locale
	oss.imbue(oldLocale);

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "eventToString() '" << oss.str() << "'";

	return oss.str();
      }

    }
  }
}
