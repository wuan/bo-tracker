
#include "exception/Base.h"
#include "hardware/pcb/V6.h"

namespace blitzortung {
  namespace hardware {
    namespace pcb {
     
      V6::V6(SerialPort& serial) :
	Base(serial)
      {
      }

      V6::~V6() {
      }

      std::auto_ptr<data::sample::Base> V6::parse(const std::vector<std::string> &fields) {
	std::auto_ptr<data::sample::Base> sample;
	
	// parse lighning event information
	if (fields[0] == "BLSEQ") {

	  // read counter value
	  int counter;
	  {
	    std::istringstream iss(fields[1]);
	    iss >> std::hex >> counter;
	  }

	  pt::ptime eventtime = gps_.getTime(counter);

	  if (gps_.isValid()) {
	    sample = parseData(eventtime, fields[2]);

	    sample->setAntennaLongitude(gps_.getLocation().getLongitude());
	    sample->setAntennaLatitude(gps_.getLocation().getLatitude());
	    sample->setGpsNumberOfSatellites(gps_.getSatelliteCount());
	    sample->setGpsStatus(gps_.getStatus());
	  } else {
	    std::cout << "GPS not valid -> no sample\n";
	  }

	} else {
	  throw exception::Base("blitzortung::hardware::pcb::V6.parse() wrong data to parse");
	}

	return sample;
      }

      short V6::parseHex(const std::string& hexval) {
	short value;

	std::stringstream ss;
	ss << std::hex << hexval;
	ss >> value;
	return value - 128;
      }

      std::auto_ptr<data::sample::Base> V6::parseData(const pt::ptime& eventtime, const std::string& data) {

	int numberOfSamples = data.size() >> 2;

	std::vector<short> xvals;
	std::vector<short> yvals;

	double maxSquare = 0.0;
	int maxIndex = -1;
	for (int i=0; i < numberOfSamples; i++) {
	  int index = i << 2;
	  short xval = parseHex(data.substr(index, 2));
	  short yval = parseHex(data.substr(index + 2, 2));

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

	std::auto_ptr<data::sample::Base> sample(new data::sample::Version1());
	sample->setTime(eventtime.time_of_day());
	sample->setOffset(maxIndex, 1);
	sample->setAmplitude(xvals[maxIndex], yvals[maxIndex], 1);

	return sample;
      }
    }
  }
}

