#include <sstream>

#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/Communication.h"

namespace blitzortung {
  namespace hardware {

    Communication::Communication(SerialPort& port) :
      serialPort_(port)
    {
    }

    Communication::~Communication() {
    }

    bool Communication::isOpen() const {
      return serialPort_.isOpen();
    }

    const unsigned int Communication::getBaudRate() const {
      return serialPort_.getBaudRate();
    }

    std::string Communication::receive() {
      return serialPort_.receive();
    }

    void Communication::send(const std::string& data) {
      serialPort_.send(data);
    }

  }
}
