#include "hardware/parsing/Samples.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      Samples::Samples(const std::vector<std::string>& fields, const hardware::gps::Base& gps) :
	Base(),
	logger_("hardware.parsing.Samples")
      {
	data::Format::CP dataFormat;
	pt::ptime eventtime;
	pt::time_duration sampleDt;

	// the counter value is always at the second position
	counter_ = parseHex(fields[1]);
	eventtime = gps.getTime(counter_);

	if (fields[0] == "BLSIG") {
	  if (fields.size() > 3) {
	    if (fields[2].size() == 2 && fields[3].size() ==2) {
	      // BLSIG type 1
	      dataFormat = data::Format::CP(new data::Format(8, 2, 1));
	      sampleDt = pt::nanoseconds(0);
	      rawData_ = fields[2] + fields[3];
	    } else if (fields[2].size() == 3 && fields[3].size() ==3) {
	      // BLSIG type 2
	      dataFormat = data::Format::CP(new data::Format(12, 2, 1));
	      sampleDt = pt::nanoseconds(0);
	      rawData_ = fields[2] + fields[3];
	    }
	  }
	} else if (fields[0] == "BLSEQ") {
	  if (fields[2].size() == 256) {
	    // BLSIG type 2
	    dataFormat = data::Format::CP(new data::Format(8, 2, 64));
	    sampleDt = pt::nanoseconds(2900);
	    rawData_ = fields[2];
	  }
	} else if (fields[0] == "BD") {
	  if (fields[2].size() == 256) {
	    // BD type 1
	    dataFormat = data::Format::CP(new data::Format(8, 2, 64));
	    sampleDt = pt::nanoseconds(2800);
	    rawData_ = fields[2];
	  }
	} else if (fields[0] == "BM") {
	  // BM type 1
	  dataFormat = data::Format::CP(new data::Format(8, 1, 128));
	  sampleDt = pt::nanoseconds(2800);
	  rawData_ = fields[2];
	}

	// parse lighning event information
	if (gps.isValid() && eventtime != pt::not_a_date_time) {
	  int numberOfEvents = rawData_.size() >> 2;

	  data::Array::AP array(new data::Array(dataFormat));

	  unsigned short hexCharsPerSample = (dataFormat->getNumberOfBitsPerSample() + 3 ) / 4;
	  unsigned short numberOfChannels = dataFormat->getNumberOfChannels();
	  int offset = 1 << (dataFormat->getNumberOfBitsPerSample() - 1);
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "#ch " << numberOfChannels << ", #chars/sample " << hexCharsPerSample << " zeroOffset " << offset;

	  int index = 0;
	  for (int sample=0; sample < numberOfEvents; sample++) {
	    for (int channel=0; channel < numberOfChannels; channel++) {
	      std::string hexString = rawData_.substr(index, hexCharsPerSample);
	      array->set(parseHex(hexString) - offset, sample, channel);
	      index += hexCharsPerSample;
	    }
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
