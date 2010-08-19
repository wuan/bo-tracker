

#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/pcb/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {
     
      Base::Base(SerialPort& serial, const gps::Type& gpsType, const data::sample::Base::Creator& sampleCreator) :
	communication_(serial),
	logger_("hardware.pcb.Base"),
	gps_(communication_, gpsType),
	sampleCreator_(sampleCreator)
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      Base::~Base() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "destroyed";
      }

      int Base::parseHex(const std::string& hexString) {
	int hexValue;

	std::istringstream iss(hexString);
	iss >> std::hex >> hexValue;

	return hexValue;
      }

      bool Base::isOpen() const {
	return communication_.isOpen();
      }

      data::sample::Base::AP Base::read() {
	std::string line = communication_.receive();


	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "read() serial input: " << line;

	int linelength = line.size();

	if (linelength > 3 && line[0] == '$' && line[linelength - 4] == '*') {
	  // valid line for checksum calculation

	  std::string content = line.substr(1, linelength - 5);

	  int transmittedChecksum;
	  std::istringstream iss(line.substr(linelength - 3, linelength - 2));
	  iss >> std::hex >> transmittedChecksum;

	  // calculate checksum of content
	  unsigned char checksum = 0;
	  if (content.length() > 1) {
	    checksum = (unsigned char)content[0];

	    for (std::string::iterator character = content.begin() + 1; character != content.end(); character++) {
	      checksum ^= (unsigned char)*character;
	    }

	    if (checksum == transmittedChecksum) {
	      // checksum values are identical

	      fields_.clear();

	      util::String::split(content, fields_, ",");

	      if (fields_[0] == "BLSEC") {
		parseGps(fields_);

		fields_.clear();
	      } else if (fields_[0] == "BLSEQ") {
		return parse(fields_);
	      } else {
		std::cout << "unknown data " << fields_[0] << std::endl;
	      }
	    }
	  }
	}

	return std::auto_ptr<data::sample::Base>();
      }

      void Base::parseGps(const std::vector<std::string>& fields) {
	gps_.parse(fields);
      }

    }
  }
}

