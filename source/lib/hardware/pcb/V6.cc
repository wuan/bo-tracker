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

      data::sample::Base::AP V6::parse(const std::vector<std::string> &fields) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() called";

	data::sample::Base::AP sample;
	
	// parse lighning event information
	if (fields[0] == "BLSEQ") {

	  // read counter value
	  int counter = parseHex(fields[1]);

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    sample = parseData(eventtime, fields[2]);

	    sample->setAntennaLongitude(gps_.getLocation().getLongitude());
	    sample->setAntennaLatitude(gps_.getLocation().getLatitude());
	    sample->setAntennaAltitude(gps_.getLocation().getAltitude());
	    sample->setGpsNumberOfSatellites(gps_.getSatelliteCount());
	    sample->setGpsStatus(gps_.getStatus());

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

      data::sample::Base::AP V6::parseData(const pt::ptime& eventtime, const std::string& data) {

	const int AD_MAX_VALUE = 128;
	const int AD_MAX_VOLTAGE = 2500;
	const int AD_THRESHOLD_VOLTAGE = 500;

	int numberOfSamples = data.size() >> 2;

	std::vector<short> xvals;
	std::vector<short> yvals;

	double maxSquare = 0.0;
	int maxIndex = -1;
	for (int i=0; i < numberOfSamples; i++) {

	  int index = i << 2;
	  short xval = parseHex(data.substr(index, 2)) - AD_MAX_VALUE;
	  short yval = parseHex(data.substr(index + 2, 2)) - AD_MAX_VALUE;

	  double square = xval * xval + yval * yval;

	  if (square > maxSquare) {
	    maxSquare = square;
	    maxIndex = i;
	  }

	  // store waveform data in arrays
	  xvals.push_back(xval);
	  yvals.push_back(yval);
	}

        float maxX = xvals[maxIndex];
	float maxY = yvals[maxIndex];

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parseData() preliminary max X: " << maxX << " Y: " << maxY << " index: " << maxIndex; 

	// correction introduced with v 16 of the original tracker software
	if ((abs(maxX) < AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE) &&
	    (abs(maxY) < AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE)) {

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "parseData() signal below threshold " << abs(maxX) << " or " << abs(maxY) << " < " << AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE;
	  maxX = AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE;
	  maxY = 0.0;
	  maxIndex = -1;

	}

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parseData() final max X: " << maxX << ", Y: " << maxY << ", index: " << maxIndex;

	data::sample::Base::AP sample(sampleCreator_());

	sample->setTime(eventtime);
	sample->setOffset(maxIndex, 1);
	sample->setAmplitude(maxX / AD_MAX_VALUE, maxY / AD_MAX_VALUE, 1);

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parseData() transmitted max X: " << maxX / AD_MAX_VALUE << ", Y: " << maxY / AD_MAX_VALUE << ", index: " << maxIndex - 1;

	return sample;
      }
    }
  }
}

