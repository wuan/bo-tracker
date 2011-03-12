#ifndef BLITZORTUNG_IPC_UNIXSOCKET_H_
#define BLITZORTUNG_IPC_UNIXSOCKET_H_

#include <boost/thread/thread.hpp>

#include "Logger.h"

namespace blitzortung {
  namespace ipc {

    class UnixSocket : private boost::noncopyable {

      private:

	//! generated ѕocket
	unsigned int socket_;

	const std::string& socketFileName_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	UnixSocket(const std::string& socketFileName);

	//! destruct socket
	virtual ~UnixSocket();
    };
  }
}

#endif
