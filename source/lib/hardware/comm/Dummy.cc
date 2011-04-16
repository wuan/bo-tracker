#include <sstream>

#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/comm/Dummy.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      Dummy::Dummy(bool keepOpen) :
	keepOpen_(keepOpen),
	logger_("hardware.comm.Dummy")
      {
	readPosition_ = 0;
      }

      Dummy::~Dummy() {
      }

      bool Dummy::isOpen() const {
	return isAvailable() || keepOpen_;
      }

      bool Dummy::isAvailable() const {
	return readPosition_ < dummyData_.size();
      }

      void Dummy::setBaudRate(const unsigned int baudRate) {
	dummyBaud_ = baudRate;
      }

      const unsigned int Dummy::getBaudRate() const {
	return dummyBaud_;
      }
      
      const std::string& Dummy::getInterfaceName() const {
	return dummyInterface_;
      }

      std::string Dummy::receive() {
	if (isAvailable())
	  return dummyData_[readPosition_++];

	sleep(1);
	return "";
      }

      void Dummy::send(const std::string& data) {
	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "send() '" << data << "'";
      }
      
      void Dummy::addReceivedLine(const std::string& line) {
	dummyData_.push_back(line);
      }
      
      const std::string Dummy::dummyInterface_ = "dummy";

    }
  }
}
