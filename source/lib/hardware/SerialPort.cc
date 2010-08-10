#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <termios.h>

#include "exception/Base.h"
#include "hardware/SerialPort.h"

namespace blitzortung {
  namespace hardware {

    SerialPort::SerialPort(const std::string& portName, const unsigned int baudRate) :
      portName_(portName),
      buffer_(""),
      isOpen_(false),
      logger_("hardware.SerialPort")
    {

      if (logger_.isInfoEnabled())
        logger_.infoStream() << "initializing serial port " << portName_.c_str() << " with " << baudRate << " baud";

      serialFd_ = open(portName_.c_str(), O_RDWR | O_NOCTTY);

      if (serialFd_ < 0) {
	std::ostringstream oss;

	oss << "blitzortung::hardware::SerialPort::open() Unable to open port " << portName_;
	throw exception::Base(oss.str());
      }

      struct termios tio;
      tio.c_iflag= IGNBRK | IGNPAR ;
      tio.c_oflag= OPOST | ONLCR ;
      tio.c_cflag= B19200 | CS8 | CLOCAL | CREAD ; 
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

    void SerialPort::setBaudRate(const unsigned int baudRate) {
      
      if (baudRate == getBaudRate())
	return;

      struct termios serialAttr;

      tcgetattr(serialFd_, &serialAttr);

      int baudBit = 0;

      switch(baudRate) {
	case 4800:
	  baudBit = B4800;
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
    
    const unsigned int SerialPort::getBaudRate() const {
      struct termios serialAttr;

      tcgetattr(serialFd_, &serialAttr);

      int baudBit = cfgetospeed(&serialAttr);

      switch(baudBit) {
	case B4800:
	  return 4800;

	case B19200:
	  return 19200;

	default:
	  std::ostringstream oss;
	  oss << "blitzortung::hardware::Serial::getBaudRate() unsupported Baud-Rate-Bit: " << baudBit;
	  throw exception::Base(oss.str());
      }
    }

    void SerialPort::send(const std::string& data) {
      int rc = write(serialFd_, data.c_str(), data.length());
      if (rc < 0)
	logger_.errorStream() << "write error in send()";
    }

    const std::string SerialPort::receive() {
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
	  logger_.warnStream() << "returned no characters in receive()";
	  break;
	}
      }

      return "";
    }

  }
}
