#ifndef BLITZORTUNG_IPC_UNIXSOCKET_H_
#define BLITZORTUNG_IPC_UNIXSOCKET_H_

#include "Logger.h"
#include "ipc/server/factory/Base.h"

namespace blitzortung {
  namespace ipc {

    class UnixSocket : private boost::noncopyable {

      private:

	//! generated ѕocket
	unsigned int socket_;

	//! name of socket file	
	const std::string socketFileName_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	UnixSocket(const std::string& socketFileName, const ipc::server::factory::Base& serverFactory);

	//! destruct socket
	virtual ~UnixSocket();
    };
  }
}

#endif
