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

    std::string Communication::readLine() {
      return serialPort_.readLine();
    }

  }
}
