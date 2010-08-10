#include "exception/Base.h"
#include "hardware/pcb/V6.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {

      V6::V6(SerialPort& serial, const gps::Type& gpsType, const data::sample::Base::Creator& sampleCreator) :
	Base(serial, gpsType, sampleCreator),
	logger_("hardware.pcb.V6")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialized";
      }

      V6::~V6() {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "deleted";
      }

      std::auto_ptr<data::sample::Base> V6::parse(const std::vector<std::string> &fields) {
	std::auto_ptr<data::sample::Base> sample;
	
	// parse lighning event information
	if (fields[0] == "BLSEQ") {

	  // read counter value
	  int counter = parseHex(fields[1]);

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    sample = parseData(eventtime, fields[2]);

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

      std::auto_ptr<data::sample::Base> V6::parseData(const pt::ptime& eventtime, const std::string& data) {

	int numberOfSamples = data.size() >> 2;

	std::vector<short> xvals;
	std::vector<short> yvals;

	double maxSquare = 0.0;
	int maxIndex = -1;
	for (int i=0; i < numberOfSamples; i++) {

	  int index = i << 2;
	  short xval = parseHex(data.substr(index, 2)) - 0x80;
	  short yval = parseHex(data.substr(index + 2, 2)) - 0x80;

	  //std::cout << " " << i << " " << xval << " " << yval << std::endl;

	  double square = xval * xval + yval * yval;

	  if (square > maxSquare) {
	    maxSquare = square;
	    maxIndex = i;
	  }

	  // store waveform data in arrays
	  xvals.push_back(xval);
	  yvals.push_back(yval);
	}	    

	std::auto_ptr<data::sample::Base> sample(sampleCreator_());

	sample->setTime(eventtime.time_of_day());
	sample->setOffset(maxIndex, 1);
	sample->setAmplitude(xvals[maxIndex], yvals[maxIndex], 1);

	return sample;
      }
    }
  }
}

