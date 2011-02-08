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

      void Tcp::send(const data::Events& events) const {
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
