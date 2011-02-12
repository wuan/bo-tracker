#include "Ticks.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      Ticks::Ticks(const std::vector<std::string> fields) {
	if (fields[0] == "BLSEC") {

	  // read counter value
	  counter_ = parseHex(fields6counter);

	  // set GPS reception status
	  setStatus(fields[2][0]);

	  // set GPS PPS time
	  time_.setSecond(fields[4] + " " + fields[3], counter);

	  // exctract actual location and altitude information
	  longitude = parseLongitude(fields[7], fields[8][0]);
	  location_.addLatitude(fields[5], fields[6][0]);
	  location_.addAltitude(fields[9]);

	  // use of value in fields[10] ???

	  // add actual satellite count to ringbuffer
	  addSatelliteCount(fields[11]);

	} else {
	  throw exception::Base("blitzortung::hardware::gps::Base.parse() wrong data to parse");
	}
	if (logger_.isDebugEnabled())
	  logger_.debug("parse() done");
      }

    }
  }
}
