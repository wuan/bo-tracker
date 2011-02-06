#include "hardware/parsing/Samples.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      Samples::Samples(const std::vector<std::string> fields, const hardware::gps::Base& gps) :
	Base()
      {
	data::Format::CP dataFormat;
	pt::ptime eventtime;
	pt::time_duration sampleDt;

	if (fields[0] == "BLSIG") {
	  eventtime = gps.getTime(parseHex(fields[1]));
	  if (fields.size() > 3) {
	    if (fields[2].size() == 2 && fields[3].size() ==2) {
	      dataFormat = data::Format::CP(new data::Format(8, 2, 1));
	      sampleDt = pt::nanoseconds(0);
	      rawData_ = fields[2] + fields[3];
	    }
	    else if (fields[2].size() == 3 && fields[3].size() ==3) {
	      dataFormat = data::Format::CP(new data::Format(12, 2, 1));
	      sampleDt = pt::nanoseconds(0);
	      rawData_ = fields[2] + fields[3];
	    }
	  } else {
	    if (fields[2].size() == 256) {
	      dataFormat = data::Format::CP(new data::Format(8, 2, 64));
	      sampleDt = pt::nanoseconds(3125);
	      rawData_ = fields[2];
	    }
	  }
	} else if (fields[0] == "BLSTR") {
	  eventtime = gps.getTime(parseHex(fields[1]));

	} else if (fields[0] == "BLSEQ") {
	  eventtime = gps.getTime(parseHex(fields[1]));
	  if (fields[2].size() == 256) {
	    dataFormat = data::Format::CP(new data::Format(8, 2, 64));
	    sampleDt = pt::nanoseconds(3125);
	    rawData_ = fields[2];
	  }

	}

	// parse lighning event information

	if (gps.isValid() && eventtime != pt::not_a_date_time) {
	  int numberOfEvents = rawData_.size() >> 2;

	  data::Array::AP array(new data::Array(dataFormat));

	  int offset = 1 << (dataFormat->getNumberOfBitsPerSample() - 1);

	  for (int i=0; i < numberOfEvents; i++) {

	    int index = i << 2;

	    short xval = parseHex(rawData_.substr(index, 2)) - offset;
	    short yval = parseHex(rawData_.substr(index + 2, 2)) - offset;

	    array->set(xval, i, 0);
	    array->set(yval, i, 1);
	  }

	  waveform_ = data::Waveform::AP(new data::Waveform(array, eventtime, sampleDt));
	  valid_ = true;
	}

      }

      data::Waveform::AP Samples::getWaveform() {
	return waveform_;
      }

      const std::string& Samples::getRawData() const {
	return rawData_;
      }

    }
  }
}
