#ifndef BLITZORTUNG_SOCKET_UNIX_H_
#define BLITZORTUNG_SOCKET_UNIX_H_

#include <boost/thread/thread.hpp>

#include "Logger.h"

namespace blitzortung {

    class Unix : private boost::noncopyable {

      private:

	//! generated ѕocket
	unsigned int socket_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct socket
	Unix(const std::string& socketFileName);

	//! destruct socket
	virtual ~Unix();
    };
}

#endif
