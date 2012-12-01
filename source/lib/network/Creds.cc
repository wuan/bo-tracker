#include "network/Creds.h"

namespace blitzortung {
  namespace network {

    Creds::Creds() :
      username_(""),
      password_(""),
      servername_(""),
      serverport_(0)
    {
    }

    void Creds::setUsername(const std::string& username) {
      username_ = username;
    }

    const std::string& Creds::getUsername() const {
      return username_;
    }

    void Creds::setPassword(const std::string& password) {
      password_ = password;
    }

    const std::string& Creds::getPassword() const {
      return password_;
    }

    void Creds::setServername(const std::string& servername) {
      servername_ = servername;
    }

    const std::string& Creds::getServername() const {
      return servername_;
    }

    void Creds::setServerport(const unsigned short serverport) {
      serverport_ = serverport;
    }

    unsigned short Creds::getServerport() const {
      return serverport_;
    }

  }
}

