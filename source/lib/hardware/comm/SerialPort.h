#ifndef BLITZORTUNG_HARDWARE_SERIALPORT_H_
#define BLITZORTUNG_HARDWARE_SERIALPORT_H_

#include <string>

#include "Logger.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      class SerialPort {
	private:

	  //! serial port device file path
	  std::string portName_;

	  //! serial port input buffer
	  std::string buffer_;

	  //! open flag for serial port
	  bool isOpen_;

	  //! serial port file descriptor
	  int serialFd_;

	  //! class logger
	  Logger logger_;

	public:
	  SerialPort(const std::string &port="/dev/ttyS0", const unsigned int baudRate = 19200);

	  virtual ~SerialPort();

	  bool isOpen() const;

	  void setBaudRate(const unsigned int);

	  const unsigned int getBaudRate() const;

	  const std::string receive();

	  void send(unsigned char);

	  void send(const std::string &);

      };

    }
  }
}

#endif
