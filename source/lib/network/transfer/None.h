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

	  //! create data string from sample
	  virtual std::string sampleToString(const data::Sample& sample) const;

	  //! send data to server
	  virtual void send(const data::Sample::VP& samples);

      };

    }
  }
}

#endif

