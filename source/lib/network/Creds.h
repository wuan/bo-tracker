#ifndef BLITZORTUNG_NETWORK_CREDS_H_
#define BLITZORTUNG_NETWORK_CREDS_H_

#include "namespaces.h"

namespace blitzortung {
  namespace network {

    class Creds : private boost::noncopyable {

      private:
	std::string username_;
	std::string password_;
	std::string servername_;
	unsigned short serverport_;

      public:
	Creds();

	virtual ~Creds();

	void setUsername(const std::string&);
	const std::string& getUsername() const;

	void setPassword(const std::string&);
	const std::string& getPassword() const;

	void setServername(const std::string&);
	const std::string& getServername() const;

	void setServerport(const unsigned short);
	const unsigned short getServerport() const;
    };
  }
}

#endif
