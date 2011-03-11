#ifndef BLITZORTUNG_SOCKET_LISTENER_H_
#define BLITZORTUNG_SOCKET_LISTENER_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "Logger.h"

namespace blitzortung {

    class Listener {

      private:

	//! socket for which this listener is active
	const unsigned int socket_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	Listener(const unsigned int socket);

	//! destruct socket
	virtual ~Listener();

	//! listener thread procedure
	void operator ()();
    };

}

#endif
