#ifndef BLITZORTUNG_NETWORK_TRANSFER_TCP_H_
#define BLITZORTUNG_NETWORK_TRANSFER_TCP_H_

#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class Tcp : public Base {

	private:

	  //! logger for this class
	  mutable Logger logger_;

	public:

	  //! create network transfer object
	  Tcp(const Creds& creds);

	  //! destroy object
	  virtual ~Tcp();

	  //! send data to server
	  void send(const data::Events& events) const;
      };

    }
  }
}

#endif
