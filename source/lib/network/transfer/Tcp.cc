#include "exception/Base.h"
#include "network/transfer/Tcp.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      Tcp::Tcp(const Creds& creds)
	: Base(creds),
	logger_("network.transfer.Tcp")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initalized()";
      }

      Tcp::~Tcp() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      std::string Tcp::sampleToString(const data::Sample& sample) const {
	const data::Sample::Waveform& wfm = sample.getWaveform();
	const data::GpsInfo& gpsInfo = sample.getGpsInfo();

	const int AD_MAX_VALUE = 128;
	const int AD_MAX_VOLTAGE = 2500;
	const int AD_THRESHOLD_VOLTAGE = 500;

        float maxX = wfm.getMaxX();
	float maxY = wfm.getMaxY();
	int maxIndex = wfm.getMaxIndex();

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parseData() preliminary max X: " << maxX << " Y: " << maxY << " index: " << maxIndex; 

	// correction introduced with v 16 of the original tracker software
	if ((abs(maxX) < AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE) &&
	    (abs(maxY) < AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE)) {

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "parseData() signal below threshold " << abs(maxX) << " or " << abs(maxY) << " < " << AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE;
	  maxX = AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE;
	  maxY = 0.0;
	  maxIndex = -1;
	}

	pt::ptime timestamp = wfm.getTime() + wfm.getTimeDelta() * maxIndex;

	std::ostringstream oss;

	pt::time_facet *timefacet = new pt::time_facet();
	timefacet->format("%Y-%m-%d %H:%M:%S.%f");
	std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

	oss << timestamp;
	oss.setf(std::ios::fixed);
	oss.precision(6);

	oss << " " << gpsInfo.getLatitude() << " " << gpsInfo.getLongitude();
	oss << " " << gpsInfo.getAltitude();
	oss << " " << creds_.getUsername() << " " << creds_.getPassword();
	oss << " " << maxX << " " << maxY;
	oss << " " << gpsInfo.getStatus() << " " << VERSION << std::endl;

	// restore original locale
	oss.imbue(oldLocale);

	return oss.str();
      }

      void Tcp::send(const data::Sample::VP& samples) const {
	int sock_id;

	sock_id = socket (AF_INET, SOCK_DGRAM, 0);

	if (sock_id == -1) {
	  logger_.warnStream() << "could not open socket";

	  return;
	}

	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(creds_.getServerport());
	serv_addr.sin_addr.s_addr = inet_addr(creds_.getServername().c_str());

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "openConnection() host: " << creds_.getServername() << " port " << creds_.getServerport();

	if (serv_addr.sin_addr.s_addr == INADDR_NONE) {
	  /* host not given by IP but by name */
	  hostent *hostinfo = gethostbyname (creds_.getServername().c_str());

	  if (hostinfo == NULL) {
	    close(sock_id);
	    logger_.warnStream() << "could not get host information for '" << creds_.getServername() << "'";

	    return;
	  }
	  memcpy((char*) &serv_addr.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
	}

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "send data to '" << creds_.getServername() << "' port " << creds_.getServerport();

	// loop through all current samples
	for (data::Sample::CVI sample = samples->begin(); sample != samples->end(); sample++) {

	  std::string data = sampleToString(*sample);

	  if (logger_.isInfoEnabled())
	    logger_.infoStream() << data.substr(0, data.size() -1);

	  if (sendto(sock_id, data.c_str(), data.size(), 0, (sockaddr*) &serv_addr, sizeof(sockaddr)) == -1) {
	    logger_.warnStream() << "() error transmitting data";
	    break;
	  }
	}

	close(sock_id);

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "() connection closed";
      }

    }
  }
}
