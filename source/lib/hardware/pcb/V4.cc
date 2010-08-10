#include "exception/Base.h"
#include "hardware/pcb/V4.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {

      V4::V4(SerialPort& serial, const gps::Type& gpsType, const data::sample::Base::Creator& sampleCreator) :
	Base(serial, gpsType, sampleCreator),
	logger_("hardware.pcb.V4")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialize";
      }

      V4::~V4() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "delete";
      }

      std::auto_ptr<data::sample::Base> V4::parse(const std::vector<std::string> &fields) {
	std::auto_ptr<data::sample::Base> sample;
	
	// parse lighning event information
	if (fields[0] == "BLSIG") {

	  int counter = parseHex(fields[1]);
	  int maxX = (parseHex(fields[2]) >> 2) - 0x200;
	  int maxY = (parseHex(fields[3]) >> 2) - 0x200;

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    sample = std::auto_ptr<data::sample::Base>(sampleCreator_());

	    sample->setTime(eventtime.time_of_day());
	    sample->setAmplitude(1, maxX, maxY);
	    sample->setAntennaLongitude(gps_.getLocation().getLongitude());
	    sample->setAntennaLatitude(gps_.getLocation().getLatitude());
	    sample->setGpsNumberOfSatellites(gps_.getSatelliteCount());
	    sample->setGpsStatus(gps_.getStatus());
	  } else {
	    logger_.warnStream() << "GPS information is not yet valid -> no sample created";
	  }

	} else {
	  logger_.errorStream() << "parse() data header '" << fields[0] << "' mismatch";
	}

	return sample;
      }
    }
  }
}

