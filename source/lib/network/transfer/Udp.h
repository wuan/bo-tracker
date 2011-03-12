#ifndef BLITZORTUNG_NETWORK_TRANSFER_UDP_H_
#define BLITZORTUNG_NETWORK_TRANSFER_UDP_H_

#include <arpa/inet.h>
#include <netdb.h>

#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class Udp : public Base {

	private:

	  //! socket id of udp connection
	  int sockId_;

	  //! socket info of udp connection
	  sockaddr_in servAddr_;

	  //! logger for this class
	  mutable Logger logger_;

	  //! open udp connection to server
	  bool openConnection();

	  //! close udp connection to server
	  void closeConnection();

	public:

	  //! create network transfer object
	  Udp(const Creds& creds);

	  //! destroy object
	  virtual ~Udp();

	  //! send data to server
	  void send(const data::Events& events);
      };

    }
  }
}

#endif
