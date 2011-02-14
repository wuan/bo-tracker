#include <sstream>

#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/comm/Serial.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      Serial::Serial(SerialPort& port) :
	serialPort_(port),
	logger_("hardware.comm.Serial")
      {
      }

      Serial::~Serial() {
      }

      unsigned char Serial::calcChecksum(const std::string& content) {
	unsigned char checksum = (unsigned char)content[0];

	for (std::string::const_iterator character = content.begin() + 1; character != content.end(); character++) {
	  checksum ^= (unsigned char)*character;
	}
	return checksum;
      }

      std::string Serial::checkLine(const std::string& line) {

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
		logger_.noticeStream() << "checkLine() checksum mismatch: '" << line << "' vs. " << oss.str() ;
	      }
	    }
	  }
	} else {
	  logger_.noticeStream() << "checkLine() invalid line '" << line << "'";
	}
	return "";
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
	std::string line;

	do {
	  line = checkLine(serialPort_.receive());
	} while (line.length() == 0);

	return line;
      }

      void Serial::send(const std::string& data) {

	unsigned char checksum = calcChecksum(data);

	std::ostringstream oss;
	oss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(checksum);

	std::string command = std::string("$") + data + std::string("*") + oss.str();

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "send() command '" << command << "'";
	
	serialPort_.send(command + std::string("\n"));
      }

    }
  }
}
