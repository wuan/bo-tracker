#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <termios.h>

#include "exception/Base.h"
#include "hardware/SerialPort.h"

namespace blitzortung {
  namespace hardware {

    SerialPort::SerialPort(const std::string& portName, const int baudRate) :
      portName_(portName),
      buffer_(""),
      isOpen_(false)
    {
      std::cout << "open serial port " << portName_ << std::endl;

      serialFd_ = open(portName_.c_str(), O_RDWR | O_NOCTTY);

      if (serialFd_ < 0) {
	std::ostringstream oss;

	oss << "blitzortung::hardware::SerialPort::open() Unable to open port " << portName_;
	throw exception::Base(oss.str());
      }

      struct termios tio;
      tio.c_iflag= IGNBRK | IGNPAR ;
      tio.c_oflag= OPOST | ONLCR ;
      tio.c_cflag= B9600 | CS8 | CLOCAL | CREAD ; 
      tio.c_lflag= 0;
      tio.c_cc[VTIME]= 0;
      tio.c_cc[VMIN]= 1;
      tcsetattr(serialFd_, TCSANOW, &tio); 

      isOpen_ = true;

      setBaudRate(baudRate);
    }

    bool SerialPort::isOpen() const {
      return isOpen_;
    }

    SerialPort::~SerialPort() {
      if (serialFd_ >= 0) {
	close(serialFd_);
      }
      isOpen_ = false;
    }

    void SerialPort::setBaudRate(const int baudRate) {

      struct termios serialAttr;

      tcgetattr(serialFd_, &serialAttr);

      int baudBit = 0;

      switch(baudRate) {
	case 9600:
	  baudBit = B9600;
	  break;

	case 19200:
	  baudBit = B19200;
	  break;

	default:
	  std::ostringstream oss;
	  oss << "blitzortung::hardware::Serial::setBaudRate() unsupported Baud-Rate " << baudRate;
	  throw exception::Base(oss.str());
      }

      cfsetospeed(&serialAttr, baudBit);

      tcsetattr(serialFd_, TCSANOW, &serialAttr);
    }
    
    const std::string SerialPort::readLine() {
      char character;

      int readlength = 0;
      while (true) {
        readlength = read(serialFd_, &character, 1);
	
        if (readlength > 0) {

	  if (character != '\n') {
	    buffer_ += character;
	  } else {
	    std::string line = buffer_;
	    buffer_.clear();
	    return line;
	  }

	} else {
	  std::cout << "returned no characters\n";
	  break;
	}
      }

      return "";
    }

  }
}
