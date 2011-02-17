#ifndef BLITZORTUNG_NETWORK_TRANSFER_NONE_H_
#define BLITZORTUNG_NETWORK_TRANSFER_NONE_H_

#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class None : public Base {

	private:

	  //! logger for this class
	  mutable Logger logger_;

	  Creds localCreds_;

	public:

	  //! create network transfer object
	  None();

	  //! destroy object
	  virtual ~None();

	  //! send data to server
	  virtual void send(const data::Events& events);

      };

    }
  }
}

#endif

