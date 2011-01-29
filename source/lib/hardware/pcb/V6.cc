#include "exception/Base.h"
#include "hardware/pcb/V6.h"
#include "data/Waveform.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {

      const pt::time_duration V6::SAMPLE_RATE = pt::nanoseconds(3125);

      V6::V6(comm::Base& comm, gps::Base& gps) :
	Base(comm, gps),
	logger_("hardware.pcb.V6")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      V6::~V6() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      data::Event::AP V6::parse(const std::vector<std::string> &fields) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() called";

	data::Event::AP event;
	
	// parse lighning event information
	if (fields[0] == "BLSEQ") {

	  // read counter value
	  int counter = parseHex(fields[1]);

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    if (logger_.isInfoEnabled())
	      logger_.infoStream() << "ѕtring data size: " << fields[2].size();

	    event = parseData(eventtime, fields[2]);

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "parse() event ready";

	  } else {
	    logger_.warnStream() << "GPS information is not yet valid -> no event created";
	  }

	} else {
	  logger_.errorStream() << "parse() data header '" << fields[0] << "' mismatch";
	}

	return event;
      }

      data::Event::AP V6::parseData(const pt::ptime& eventtime, const std::string& data) {

	int numberOfEvents = data.size() >> 2;

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "parseData() " << numberOfEvents;
     
	data::Array::AP array(new data::Array(dataFormat_));

	for (int i=0; i < numberOfEvents; i++) {

	  int index = i << 2;

	  unsigned short xval = parseHex(data.substr(index, 2));
	  unsigned short yval = parseHex(data.substr(index + 2, 2));

	  array->set(xval, i, 0);
	  array->set(yval, i, 1);
	}

	data::Waveform::AP wfm(new data::Waveform(array, eventtime, SAMPLE_RATE));

	data::Event::AP event = data::Event::AP(new data::Event(wfm, gps_.getInfo()));

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parseData() done";

	return event;
      }
    }
  }
}

