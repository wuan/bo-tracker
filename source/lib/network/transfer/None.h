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

	public:

	  //! create network transfer object
	  None(const Creds& creds);

	  //! destroy object
	  virtual ~None();

	  //! create data string from event
	  virtual std::string eventToString(const data::Event& event) const;

	  //! send data to server
	  virtual void send(const data::Event::VP& events);

      };

    }
  }
}

#endif

