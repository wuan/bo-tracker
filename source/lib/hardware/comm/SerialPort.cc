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

      unsigned char SerialPort::calcChecksum(const std::string& content) {
	unsigned char checksum = (unsigned char)content[0];

	for (std::string::const_iterator character = content.begin() + 1; character != content.end(); character++) {
	  checksum ^= (unsigned char)*character;
	}
	return checksum;
      }

      std::string SerialPort::checkLine(const std::string& line) {

	int linelength = line.size();

	if (linelength > 3 && line[0] == '$' && line[linelength - 4] == '*') {
	  // valid line for checksum calculation

	  std::string content = line.substr(1, linelength - 5);

	  int transmittedChecksum;
	  std::istringstream iss(line.substr(linelength - 3, linelength - 2));
	  iss >> std::hex >> transmittedChecksum;

	  // calculate checksum of content
	  if (content.length() > 1) {
	    unsigned char checksum = calcChecksum(content);

	    if (checksum == transmittedChecksum) {
	      // checksum values are identical
	      return content;
	    } else {
	      if (logger_.isDebugEnabled()) {
		std::ostringstream oss;
		oss << std::hex << (int)checksum;
		logger_.debugStream() << "read() checksum mismatch: '" << line << "' vs. " << oss.str() ;
	      }
	    }
	  }
	}
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
}
