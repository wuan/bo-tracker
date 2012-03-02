#include <iostream>
#include <sstream>

#include <fcntl.h>
#include <termios.h>

#include "exception/Base.h"
#include "hardware/comm/SerialPort.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      SerialPort::SerialPort(const std::string& portName, const unsigned int baudRate) :
	portName_(portName),
	buffer_(""),
	isOpen_(false),
	logger_("hardware.comm.SerialPort")
      {

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "initializing serial port " << portName_.c_str() << " with " << baudRate << " baud";

	serialFd_ = open(portName_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

	if (serialFd_ < 0) {
	  std::ostringstream oss;

	  oss << "blitzortung::hardware::SerialPort::open() Unable to open port " << portName_;
	  throw exception::Base(oss.str());
	}

	// use serial port in blocking mode
	fcntl(serialFd_, F_SETFL, 0);

	struct termios options;

	tcgetattr(serialFd_, &options);

	// set baud rate
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);
	options.c_cflag |= (CLOCAL | CREAD) ; 

	// No parity (8N1)
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;

	// set character size
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// disable hardware flow control
	options.c_cflag &= ~CRTSCTS;

	// ignore break condition and parity errors 
	options.c_iflag = (IGNBRK | IGNPAR);

	// postprocess output and map LF to CRLF 
	options.c_oflag = OPOST | ONLCR ;

	// use raw input (no line based editing, echo, etc.)
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	// set control characters
	// time to wait for data (tenth of seconds)
	options.c_cc[VTIME]= 0;
	// minimum number of characters to read
	options.c_cc[VMIN]= 1;

	tcsetattr(serialFd_, TCSANOW, &options); 

	isOpen_ = true;


	setBaudRate(baudRate);
      }

      bool SerialPort::isOpen() const {
	return isOpen_;
      }

      SerialPort::~SerialPort() {
	logger_.infoStream() << "delete port with fd " << serialFd_;
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

	speed_t baudBit = 0;

	switch(baudRate) {
	  case 4800:
	    baudBit = B4800;
	    break;

	  case 9600:
	    baudBit = B9600;
	    break;

	  case 19200:
	    baudBit = B19200;
	    break;

	  case 38400:
	    baudBit = B38400;
	    break;

	  case 115200:
	    baudBit = B115200;
	    break;

	  case 230400:
	    baudBit = B230400;
	    break;

#ifdef B250000
	  case 250000:
	    baudBit = B250000;
	    break;
#endif

	  case 500000:
	    baudBit = B500000;
	    break;

	  default:
	    std::ostringstream oss;
	    oss << "blitzortung::hardware::comm::SerialPort::setBaudRate() " << portName_ << " unsupported Baud-Rate " << baudRate;
	    throw exception::Base(oss.str());
	}

	cfsetospeed(&serialAttr, baudBit);

	tcsetattr(serialFd_, TCSANOW, &serialAttr);
      }

      const unsigned int SerialPort::getBaudRate() const {
	struct termios serialAttr;

	tcgetattr(serialFd_, &serialAttr);

	speed_t baudBit = cfgetospeed(&serialAttr);

	//if (logger_.isDebugEnabled())
	//  logger_.debugStream() << "getBaudRate() bit " << baudBit;

	switch(baudBit) {
	  case B4800:
	    return 4800;

	  case B9600:
	    return 9600;

	  case B19200:
	    return 19200;

	  case B38400:
	    return 38400;

	  case B115200:
	    return 115200;

	  case B230400:
	    return 230400;

#ifdef B250000
	  case B250000:
	    return 250000;
#endif

	  case B500000:
	    return 500000;

	  default:
	    std::ostringstream oss;
	    oss << "blitzortung::hardware::comm::SerialPort::getBaudRate() " << portName_ << " unsupported Baud-Rate-Bit: " << baudBit;
	    logger_.errorStream() << oss.str();
	    throw exception::Base(oss.str());
	}
      }
      
      const std::string& SerialPort::getPortName() const {
	return portName_;
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
	    logger_.errorStream() << "read error in receive() " << readlength;
	    exit(1);
	  }
	}

	return "";
      }

      void SerialPort::flushInput() const {
	tcflush(serialFd_, TCIFLUSH);
      }


    }
  }
}
