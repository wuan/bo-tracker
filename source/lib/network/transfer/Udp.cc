#include "exception/Base.h"
#include "network/transfer/Udp.h"
#include "data/MEvent.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      Udp::Udp(const Creds& creds) :
	Base(creds),
	sockId_(-1),
	servAddr_(),
	logger_("network.transfer.Udp")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initalized()";
      }

      Udp::~Udp() {
	closeConnection();
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      bool Udp::openConnection() {
	if (sockId_ < 0) {
	  sockId_ = socket(AF_INET, SOCK_DGRAM, 0);

	  if (sockId_ == -1) {
	    logger_.warnStream() << "could not open socket";
	    return false;
	  }

	  servAddr_.sin_family = AF_INET;
	  servAddr_.sin_port = htons(creds_.getServerport());
	  servAddr_.sin_addr.s_addr = inet_addr(creds_.getServername().c_str());

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "openConnection() host: " << creds_.getServername() << " port " << creds_.getServerport();

	  if (servAddr_.sin_addr.s_addr == INADDR_NONE) {
	    /* host not given by IP but by name */
	    hostent *hostinfo = gethostbyname (creds_.getServername().c_str());

	    if (hostinfo == NULL) {
	      logger_.warnStream() << "could not get host information for '" << creds_.getServername() << "'";
	      closeConnection();
	      return false;
	    }
	    memcpy((char*) &servAddr_.sin_addr.s_addr, hostinfo->h_addr, hostinfo->h_length);
	  }
	}
	return true;
      }

      void Udp::closeConnection() {
	if (sockId_ >= 0) {
	  close(sockId_);
	  sockId_ = -1;

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "closeConnection() connection closed";
	}
      }

      void Udp::send(const data::Events& events) {

	if (openConnection()) {

	  // loop through all current events
	  for (data::Event::CVI event = events.begin(); event != events.end(); event++) {

	    std::string data = eventToString(*event);

	    if (logger_.isInfoEnabled())
	      logger_.infoStream() << "send data to '" << creds_.getServername() << "' port " << creds_.getServerport() << ": '" << data << "'";

	    data.append("\n");

	    if (sendto(sockId_, data.c_str(), data.size(), 0, (sockaddr*) &servAddr_, sizeof(sockaddr)) == -1) {
	      logger_.warnStream() << "() error transmitting data";
	      break;
	    }
	  }
	}
      }

    }
  }
}
