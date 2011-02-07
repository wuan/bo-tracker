#include <sstream>

#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/comm/Dummy.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      Dummy::Dummy() :
	logger_("hardware.comm.Dummy")
      {
	readPosition_ = 0;
      }

      Dummy::~Dummy() {
      }

      bool Dummy::isOpen() const {
	return readPosition_ < dummyData_.size();
      }

      void Dummy::setBaudRate(const unsigned int baudRate) {
	dummyBaud_ = baudRate;
      }

      const unsigned int Dummy::getBaudRate() const {
	return dummyBaud_;
      }

      std::string Dummy::receive() {
	if (isOpen())
	  return dummyData_[readPosition_++];

	return "";
      }

      void Dummy::send(const std::string& data) {
	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "send() '" << data << "'";
      }
      
      void Dummy::addReceivedLine(const std::string& line) {
	dummyData_.push_back(line);
      }

    }
  }
}
