

#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/pcb/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {
     
      Base::Base(comm::Base& comm, gps::Base& gps, const data::sample::Base::Creator& sampleCreator) :
	comm_(comm),
	logger_("hardware.pcb.Base"),
	gps_(gps),
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
	return comm_.isOpen();
      }

      data::sample::Base::AP Base::read() {
	std::string line = comm_.receive();

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "read() serial input: " << line;


	if (line.length() > 0) {

	  fields_.clear();

	  util::String::split(line, fields_, ",");

	  if (fields_[0] == "BLSEC") {
	    parseGps(fields_);

	    fields_.clear();
	  } else if (fields_[0] == "BLSEQ" || fields_[0] == "BLSIG") {
	    return parse(fields_);
	  } else {
	    std::cout << "unknown data " << fields_[0] << std::endl;
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

