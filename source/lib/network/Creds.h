#ifndef BLITZORTUNG_NETWORK_CREDS_H_
#define BLITZORTUNG_NETWORK_CREDS_H_

#include "namespaces.h"

namespace blitzortung {
  namespace network {

    class Creds : private boost::noncopyable {

      private:

	//! user name of blitzortung project
	std::string username_;

	//! password of user of blitzortung project
	std::string password_;

	//! name of blitzortung data target server
	std::string servername_;

	//! tcp port of blitzortung data target service
	unsigned short serverport_;

      public:

	//! constructor
	Creds();

	//! destructor
	virtual ~Creds();

	//! set user name of blitzortung participant
	void setUsername(const std::string&);

	//! get user name of blitzortung participant
	const std::string& getUsername() const;

	//! set password of blitzortung participant
	void setPassword(const std::string&);

	//! get password of blitzortung participant
	const std::string& getPassword() const;

	//! set name of blitzortung data target server
	void setServername(const std::string&);
	
	//! get name of blitzortung data target server;
	const std::string& getServername() const;

	//! set tcp port of blitzortung data target service
	void setServerport(const unsigned short);

	//! get tcp port of blitzortung data target service
	const unsigned short getServerport() const;

    };
  }
}

#endif
