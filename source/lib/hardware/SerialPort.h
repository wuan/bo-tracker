#ifndef BLITZORTUNG_HARDWARE_SERIALPORT_H_
#define BLITZORTUNG_HARDWARE_SERIALPORT_H_

#include <string>

namespace blitzortung {
  namespace hardware {

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

      public:
	SerialPort(const std::string &port="/dev/ttyS0", const int baudRate = 19200);

	virtual ~SerialPort();

	bool isOpen() const;

	void setBaudRate(const int);

	const std::string readLine();
	
	void writeByte(unsigned char);

	void write(const std::string &);

    };

  }
}

#endif
