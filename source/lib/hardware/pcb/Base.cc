

#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/pcb/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {
     
      Base::Base(comm::Base& comm, gps::Base& gps, const data::Format::CP& dataFormat) :
	comm_(comm),
	gps_(gps),
	dataFormat_(dataFormat),
	logger_("hardware.pcb.Base")
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
	return comm_.isOpen();
      }

      data::Event::AP Base::read() {
	std::string line = comm_.receive();

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "read() input: " << line;

	if (line.length() > 0) {

	  fields_.clear();

	  util::String::split(line, fields_, ",");

	  if (fields_[0] == "BLSEC") {
	    parseGps(fields_);
	  } else if (fields_[0] == "BLSEQ" || fields_[0] == "BLSIG") {
	    return parse(fields_);
	  } else {
	    logger_.warnStream() << "unknown data " << fields_[0];
	  }
	}
	return data::Event::AP();
      }

      void Base::parseGps(const std::vector<std::string>& fields) {
	gps_.parse(fields);
      }

    }
  }
}

