#include "hardware/pcb/V4.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {

      V4::V4(comm::Base& serial, gps::Base& gps, const data::EventFactory& eventFactory) :
	Base(serial, gps, eventFactory),
	logger_("hardware.pcb.V4")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      V4::~V4() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      data::Event::AP V4::parse(const std::vector<std::string> &fields) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() called";

	data::Event::AP event;
	
	// parse lighning event information
	if (fields[0] == "BLSIG") {

	  int counter = parseHex(fields[1]);
	  int maxX = (parseHex(fields[2]) >> 2) - 0x200;
	  int maxY = (parseHex(fields[3]) >> 2) - 0x200;

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "parse() signal: counter " << counter << " maxX " << maxX << " maxY " << maxY;

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    data::GpsInfo::AP gpsInfo(new data::GpsInfo(gps_));

	    data::Waveform::AP waveform(new data::Event::Waveform(eventtime));
	    waveform->add(maxX/1023.0, maxY/1023.0);

	    event = eventFactory_.createEvent(waveform, gpsInfo);
	  } else {
	    logger_.warnStream() << "GPS information is not yet valid -> no event created";
	  }

	} else {
	  logger_.errorStream() << "parse() data header '" << fields[0] << "' mismatch";
	}

	return event;
      }

      void V4::parseGps(const std::vector<std::string>& fields) {
	std::vector<std::string> reorderedFields;

	reorderedFields.push_back(fields[0]);
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

