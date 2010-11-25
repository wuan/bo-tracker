#ifndef BLITZORTUNG_NETWORK_TRANSFER_UDP_H_
#define BLITZORTUNG_NETWORK_TRANSFER_UDP_H_

#include "network/transfer/Base.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class Udp : public Base {

	private:

	  //! logger for this class
	  mutable Logger logger_;

	public:

	  //! create network transfer object
	  Udp(const Creds& creds);

	  //! destroy object
	  virtual ~Udp();

	  //! create data string from sample
	  virtual std::string sampleToString(const data::Sample& sample) const;

	  //! send data to server
	  void send(const data::Sample::VP& samples) const;
      };

    }
  }
}

#endif
