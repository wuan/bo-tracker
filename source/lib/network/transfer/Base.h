#ifndef BLITZORTUNG_NETWORK_TRANSFER_BASE_H_
#define BLITZORTUNG_NETWORK_TRANSFER_BASE_H_

#define VERSION                 "DebianTracker&nbsp;Ver.&nbsp;0.9.1"

#include <arpa/inet.h>
#include <netdb.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/condition.hpp>

#include "data/Events.h"
#include "network/Creds.h"
#include "Logger.h"

namespace blitzortung {
  namespace network {
    namespace transfer {

      class Base {

	public:
	  typedef std::auto_ptr<Base> AP;

	private:

	  //! logger for this class
	  mutable Logger logger_;

	protected:

	  //! reference to credentials
	  const network::Creds& creds_;
	
	public:

	  //! create network transfer object
	  Base(const Creds& creds);

	  //! destroy object
	  virtual ~Base();

	  //! create data string from event
	  virtual std::string eventToString(const data::Event& event) const = 0;

	  //! send data to server
	  virtual void send(const data::Events& events) const = 0;

      };

    }
  }
}

#endif

