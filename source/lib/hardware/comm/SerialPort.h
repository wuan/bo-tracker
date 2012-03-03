#ifndef BLITZORTUNG_HARDWARE_SERIALPORT_H_
#define BLITZORTUNG_HARDWARE_SERIALPORT_H_

#include "namespaces.h"

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      class SerialPort {

	public:

	  typedef std::unique_ptr<SerialPort> AP;

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
	  mutable Logger logger_;

	public:
	
	  //! constructor
	  SerialPort(const std::string& port="/dev/ttyS0", const unsigned int baudRate = 19200);

	  // make object nocopyable
	  SerialPort(SerialPort &) = delete;
	  SerialPort operator=(SerialPort&) = delete;

	  //! destructor
	  virtual ~SerialPort();

	  //! returns true if serial port is open
	  bool isOpen() const;

	  //! set baud rate of serial port
	  void setBaudRate(const unsigned int);

	  //! returns the actual baud rate of the serial port
	  const unsigned int getBaudRate() const;
	  
	  //! return the name of the serial port device
	  const std::string& getPortName() const;

	  //!  returns a complete line of received data, blocks until data is arrived 
	  const std::string receive();

	  //! send single character via serial port
	  void send(unsigned char);

	  //! send string via serial port
	  void send(const std::string&);

	  //! flush input buffer of serial port	  
	  void flushInput() const;

      };

    }
  }
}

#endif
