#include "exception/Base.h"
#include "hardware/pcb/V6.h"
#include "data/Waveform.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {

      const pt::time_duration V6::SAMPLE_RATE = pt::nanoseconds(3125);

      V6::V6(comm::Base& comm, gps::Base& gps, const data::SampleFactory& sampleFactory) :
	Base(comm, gps, sampleFactory),
	logger_("hardware.pcb.V6")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      V6::~V6() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      data::Sample::AP V6::parse(const std::vector<std::string> &fields) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() called";

	data::Sample::AP sample;
	
	// parse lighning event information
	if (fields[0] == "BLSEQ") {

	  // read counter value
	  int counter = parseHex(fields[1]);

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    if (logger_.isInfoEnabled())
	      logger_.infoStream() << "ѕtring data size: " << fields[2].size();

	    sample = parseData(eventtime, fields[2]);

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "parse() sample ready";

	  } else {
	    logger_.warnStream() << "GPS information is not yet valid -> no sample created";
	  }

	} else {
	  logger_.errorStream() << "parse() data header '" << fields[0] << "' mismatch";
	}

	return sample;
      }

      data::Sample::AP V6::parseData(const pt::ptime& eventtime, const std::string& data) {

	const int AD_MAX_VALUE = 128;

	int numberOfSamples = data.size() >> 2;

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "parseData() " << numberOfSamples;
       
	data::Sample::Waveform::AP wfm(new data::Sample::Waveform(eventtime, SAMPLE_RATE));

	for (int i=0; i < numberOfSamples; i++) {

	  int index = i << 2;

	  uns xval = parseHex(data.substr(index, 2));
	  short yval = parseHex(data.substr(index + 2, 2));

	  wfm->add(xval, yval);
	}

	data::Sample::AP sample = sampleFactory_.createSample(wfm, gps_.getInfo());

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parseData() done";

	return sample;
      }
    }
  }
}

