#include "exception/Base.h"
#include "network/transfer/Udp.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      Udp::Udp(const Creds& creds)
	: Base(creds),
	logger_("network.transfer.Udp")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initalized()";
      }

      Udp::~Udp() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      std::string Udp::eventToString(const data::Event& event) const {
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
	oss << " " << gpsInfo.getStatus() << " " << 2 << " " << 64 << " " << 8;
	oss << " " << wfm.getTimeDelta().total_nanoseconds() << " ";
	oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0');

	for (unsigned int i = 0; i < wfm.getNumberOfSamples(); i++) {
	  oss << int(wfm.get(i, 0)) + (1 << 7) << int(wfm.get(i, 1)) + (1 <<7);
	}

	oss << std::nouppercase << " " << VERSION << std::endl;

	// restore original locale
	oss.imbue(oldLocale);

	return oss.str();
      }

      void Udp::send(const data::Events& events) const {
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

	// loop through all current events
	for (data::Event::CVI event = events.begin(); event != events.end(); event++) {

	  std::string data = eventToString(*event);

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
