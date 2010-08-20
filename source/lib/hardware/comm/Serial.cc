#include <sstream>

#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/comm/Serial.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      Serial::Serial(SerialPort& port) :
	serialPort_(port)
      {
      }

      Serial::~Serial() {
      }

      bool Serial::isOpen() const {
	return serialPort_.isOpen();
      }

      void Serial::setBaudRate(const unsigned int baudRate) {
	serialPort_.setBaudRate(baudRate);
      }

      const unsigned int Serial::getBaudRate() const {
	return serialPort_.getBaudRate();
      }

      std::string Serial::receive() {
	return serialPort_.receive();
      }

      void Serial::send(const std::string& data) {
	serialPort_.send(data);
      }

    }
  }
}
