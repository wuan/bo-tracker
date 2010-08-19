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
	  logger_.debugStream() << "initialized";
      }

      V4::~V4() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      data::sample::Base::AP V4::parse(const std::vector<std::string> &fields) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() called";

	data::sample::Base::AP sample;
	
	// parse lighning event information
	if (fields[0] == "BLSIG") {

	  int counter = parseHex(fields[1]);
	  int maxX = (parseHex(fields[2]) >> 2) - 0x200;
	  int maxY = (parseHex(fields[3]) >> 2) - 0x200;

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "parse() signal: counter " << counter << " maxX " << maxX << " maxY " << maxY;

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "parse() create sample";
	    sample = data::sample::Base::AP(sampleCreator_());

	    sample->setTime(eventtime);
	    sample->setOffset(0, 1);
	    sample->setAmplitude(maxX, maxY, 1);
	    sample->setAntennaLongitude(gps_.getLocation().getLongitude());
	    sample->setAntennaLatitude(gps_.getLocation().getLatitude());
	    sample->setGpsNumberOfSatellites(gps_.getSatelliteCount());
	    sample->setGpsStatus(gps_.getStatus());

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "parse() create sample DONE";
	  } else {
	    logger_.warnStream() << "GPS information is not yet valid -> no sample created";
	  }

	} else {
	  logger_.errorStream() << "parse() data header '" << fields[0] << "' mismatch";
	}

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() return sample " << sample.get();
	return sample;
      }

      void V4::parseGps(const std::vector<std::string>& fields) {
	std::vector<std::string> reorderedFields;

	reorderedFields.push_back("$BLSEC");
	reorderedFields.push_back(fields[8]);
	reorderedFields.push_back(fields[3]);
	reorderedFields.push_back(fields[1]);
	reorderedFields.push_back(fields[2]);
	reorderedFields.push_back(fields[4]);
	reorderedFields.push_back(fields[5]);
	reorderedFields.push_back(fields[6]);
	reorderedFields.push_back(fields[7]);
	reorderedFields.push_back("0");
	reorderedFields.push_back("?");
	reorderedFields.push_back("0");

	if (logger_.isDebugEnabled()) {
	  std::ostringstream oss;

	  for (std::vector<std::string>::iterator field = reorderedFields.begin(); field != reorderedFields.end(); field++)
	    oss << *field << ",";

	  logger_.debugStream() << "parseGps() reordered gps data: '" << oss.str() << "'";
	}

	gps_.parse(reorderedFields);
      }
    }
  }
}

