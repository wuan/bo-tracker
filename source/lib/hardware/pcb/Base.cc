

#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/pcb/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {
     
      Base::Base(SerialPort& serial, const gps::Type& gpsType) :
	communication_(serial),
	gps_(communication_, gpsType)
      {
      }

      Base::~Base() {
      }

      bool Base::isOpen() const {
	return communication_.isOpen();
      }

      std::auto_ptr<data::sample::Base> Base::read() {
	std::string line = communication_.receive();

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
		gps_.parse(fields_);

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

    }
  }
}

