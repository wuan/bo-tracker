#include "hardware/parsing/Samples.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      Samples::Samples(const std::vector<std::string>& fields, const hardware::gps::Base& gps) :
	Base(),
	logger_("hardware.parsing.Samples")
      {
	valid_ = false;

	pt::ptime eventtime = gps.getTime(counter_);

	if (gps.isValid() && eventtime != pt::not_a_date_time) {
	  // the counter value is always at the second position
	  counter_ = parseHex(fields[1]);

	  if (fields[0] == "BLSIG") {
	    if (fields.size() > 3) {
	      if (fields[2].size() == 2 && fields[3].size() ==2) {
		// BLSIG type 1
		setWaveform(FORMAT_8_2_1, eventtime, pt::nanoseconds(0), fields[2] + fields[3]);
	      } else if (fields[2].size() == 3 && fields[3].size() ==3) {
		// BLSIG type 2
		setWaveform(FORMAT_12_2_1, eventtime, pt::nanoseconds(0), fields[2] + fields[3]);
	      }
	    }
	  } else if (fields[0] == "BLSEQ") {
	    if (fields[2].size() == 256) {
	      // BLSIG type 2
	      setWaveform(FORMAT_8_2_64, eventtime, pt::nanoseconds(2900), fields[2]);
	    }
	  } else if (fields[0] == "BD") {
	    if (fields[2].size() == 256) {
	      // BD type 1
	      setWaveform(FORMAT_8_2_64, eventtime, pt::nanoseconds(2800), fields[2]);
	    }
	  } else if (fields[0] == "BM") {
	    // BM type 1
	    setWaveform(FORMAT_8_1_128, eventtime, pt::nanoseconds(2800), fields[2]);
	  }
	}

	if (! valid_) {
	  waveform_.reset();

	  std::string line;
	  for (std::vector<std::string>::const_iterator field = fields.begin(); field != fields.end(); field++)
	    line += *field + " ";

	  logger_.warnStream() << "Ticks() could not parse sample '" << line << "'";
	}
      }

      void Samples::setWaveform(const data::Format& format, const pt::ptime& eventtime, const pt::time_duration& sampleDt, const std::string&& rawData) {
	rawData_ = rawData;

	// parse lighning event information
	if (format.isValid()) {
	  int numberOfEvents = rawData.size() >> 2;

	  data::Array::AP array(new data::Array(format));

	  unsigned short hexCharsPerSample = (format.getNumberOfBitsPerSample() + 3 ) / 4;
	  unsigned short numberOfChannels = format.getNumberOfChannels();

	  int offset = format.getSampleZeroOffset();

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "#ch " << numberOfChannels << ", #chars/sample " << hexCharsPerSample << " zeroOffset " << offset;

	  int index = 0;
	  for (int sample=0; sample < numberOfEvents; sample++) {
	    for (int channel=0; channel < numberOfChannels; channel++) {
	      std::string hexString = rawData_.substr(index, hexCharsPerSample);
	      array->set(int(offset + parseHex(hexString)), sample, channel);
	      index += hexCharsPerSample;
	    }
	  }

	  waveform_ = data::Waveform::AP(new data::Waveform(array, eventtime, sampleDt));
	  valid_ = true;

	}
      }

      data::Waveform::AP Samples::getWaveform() {
	return std::move(waveform_);
      }

      const std::string& Samples::getRawData() const {
	return rawData_;
      }

      const data::Format Samples::FORMAT_8_2_1(8,2,1);
      const data::Format Samples::FORMAT_12_2_1(12,2,1);
      const data::Format Samples::FORMAT_8_2_64(8,2,64);
      const data::Format Samples::FORMAT_8_1_128(8,1,128);

    }
  }
}
