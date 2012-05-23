#include "data/Format.h"
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

	if (gps.isValid()) {
	  // the counter value is always at the second position
	  counter_ = parseHex(fields[1]);

	  pt::ptime eventtime = gps.getTime(counter_);

	  if (eventtime != pt::not_a_date_time) {

	    if (fields[0] == "BLSIG") {
	      if (fields.size() > 3) {
		if (fields[2].size() == 2 && fields[3].size() ==2) {
		  // BLSIG type 1
		  setWaveform(FORMAT_8_2_1, eventtime, pt::nanoseconds(0), fields[2] + fields[3]);
		} else if (fields[2].size() == 3 && fields[3].size() ==3) {
		  // BLSIG type 2
		  setWaveform(FORMAT_12_2_1, eventtime, pt::nanoseconds(0), std::move(fields[2] + fields[3]), 12);
		} else {
		  logger_.warnStream() << "Samples() BLSIG field size mismatch 2: '" << fields[2] << "', 3: '" << fields[3] << "'";
		}
	      } else {
		logger_.warnStream() << "Samples() BLSIG total number of fields mismatch: " << fields.size();
	      }
	    } else if (fields[0] == "BLSEQ") {
	      if (fields[2].size() == 256) {
		// BLSIG type 2
		setWaveform(FORMAT_8_2_64, eventtime, pt::nanoseconds(2900), std::move(fields[2]));
	      } else {
		logger_.warnStream() << "Samples() BLSEQ field 2 size mismatch: " << fields[2].size() << " vs. 256";
	      }
	    } else if (fields[0] == "BD") {
	      if (fields[2].size() == 256) {
		// BD type
		setWaveform(FORMAT_8_2_64, eventtime, pt::nanoseconds(2800), std::move(fields[2]));
	      } else {
		logger_.warnStream() << "Samples() BD field 2 size mismatch: " << fields[2].size() << " vs. 256";
	      }
	    } else if (fields[0] == "BM") {
	      if (fields[2].size() == 256) {
		// BM type
		setWaveform(FORMAT_8_1_128, eventtime, pt::nanoseconds(2800), std::move(fields[2]));
	      } else {
		logger_.warnStream() << "Samples() BM field 2 size mismatch: " << fields[2].size() << " vs. 256";
	      }
	    } else if (fields[0] == "L") {
	      if (fields[2].size() == 1024) {
		// L type
		setWaveform(FORMAT_8_2_256, eventtime, pt::nanoseconds(1950), std::move(fields[2]));
	      } else {
		logger_.warnStream() << "Samples() BD field 2 size mismatch: " << fields[2].size() << " vs. 256";
	      }
	    } else {
	      logger_.warnStream() << "Samples() unknown sample type '" << fields[0] << "'";
	    }
	  } else {
	    logger_.warnStream() << "Samples() invalid event time";
	  }

	  if (! valid_) {
	    waveform_.reset();

	    std::string line;
	    for (std::vector<std::string>::const_iterator field = fields.begin(); field != fields.end(); field++)
	      line += *field + " ";

	    logger_.warnStream() << "Samples() could not parse '" << line << "'";
	  }
	}
      }

      void Samples::setWaveform(const data::Format& format, const pt::ptime& eventtime, const pt::time_duration&& sampleDt, const std::string&& rawData, unsigned char numberOfBitsPerSample) {
	if (numberOfBitsPerSample == 0) {
	  numberOfBitsPerSample = format.getNumberOfBytesPerSample() * 8;
	}

	std::ostringstream oss;
	oss << (int)format.getNumberOfChannels() << " " << format.getNumberOfSamples() << " "  << (int)numberOfBitsPerSample <<  " " << rawData;
	rawData_ = oss.str();

	// parse lighning event information
	if (format.isValid()) {
	  int numberOfEvents = rawData.size() >> 2;

	  waveform_ = format.createWaveform(eventtime, sampleDt);

	  unsigned short hexCharsPerSample = (numberOfBitsPerSample + 3 ) / 4;
	  unsigned short numberOfChannels = format.getNumberOfChannels();

	  int offset =  -(1 << (numberOfBitsPerSample - 1));

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "#ch " << numberOfChannels << ", #chars/sample " << hexCharsPerSample << " zeroOffset " << offset;

	  int index = 0;
	  for (int sample=0; sample < numberOfEvents; sample++) {
	    for (int channel=0; channel < numberOfChannels; channel++) {
	      std::string hexString = rawData.substr(index, hexCharsPerSample);
	      waveform_->setInt(int(offset + parseHex(hexString)), sample, channel);
	      index += hexCharsPerSample;
	    }
	  }

	  valid_ = true;
	} else {
	  logger_.warnStream() << "invalid format " << format;
	}
      }

      data::Waveform::AP Samples::getWaveform() {
	return std::move(waveform_);
      }

      const std::string& Samples::getRawData() const {
	return rawData_;
      }

      const data::Format Samples::FORMAT_8_2_1(1,2,1);
      const data::Format Samples::FORMAT_12_2_1(2,2,1);
      const data::Format Samples::FORMAT_8_2_64(1,2,64);
      const data::Format Samples::FORMAT_8_1_128(1,1,128);
      const data::Format Samples::FORMAT_8_2_256(1,2,256);
    }
  }
}
