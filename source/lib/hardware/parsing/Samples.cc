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
	      dataFormat = FORMAT_8_2_1;
	      sampleDt = pt::nanoseconds(0);
	      rawData_ = fields[2] + fields[3];
	    } else if (fields[2].size() == 3 && fields[3].size() ==3) {
	      // BLSIG type 2
	      dataFormat = FORMAT_12_2_1;
	      sampleDt = pt::nanoseconds(0);
	      rawData_ = fields[2] + fields[3];
	    }
	  }
	} else if (fields[0] == "BLSEQ") {
	  if (fields[2].size() == 256) {
	    // BLSIG type 2
	    dataFormat = FORMAT_8_2_64;
	    sampleDt = pt::nanoseconds(2900);
	    rawData_ = fields[2];
	  }
	} else if (fields[0] == "BD") {
	  if (fields[2].size() == 256) {
	    // BD type 1
	    dataFormat = FORMAT_8_2_64;
	    sampleDt = pt::nanoseconds(2800);
	    rawData_ = fields[2];
	  }
	} else if (fields[0] == "BM") {
	  // BM type 1
	  dataFormat = FORMAT_8_1_128;
	  sampleDt = pt::nanoseconds(2800);
	  rawData_ = fields[2];
	}

	// parse lighning event information
	if (dataFormat.get() != 0 && gps.isValid() && eventtime != pt::not_a_date_time) {
	  int numberOfEvents = rawData_.size() >> 2;

	  data::Array::AP array(new data::Array(dataFormat));

	  unsigned short hexCharsPerSample = (dataFormat->getNumberOfBitsPerSample() + 3 ) / 4;
	  unsigned short numberOfChannels = dataFormat->getNumberOfChannels();

	  int offset = dataFormat->getSampleZeroOffset();

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

	  if (waveform_->getAmplitude(waveform_->getMaxIndex() >= 0.2))
	    valid_ = true;
	  else {
	    valid_ = false;
	    logger_.noticeStream() << "low amplitude waveform rejected";
	  }

	} else {
	  waveform_.reset();

	  std::string line;
	  for (std::vector<std::string>::const_iterator field = fields.begin(); field != fields.end(); field++)
	    line += *field + " ";

	  logger_.warnStream() << "Ticks() could not parse sample '" << line << "'";
	  valid_ = false;
	}
      }

      data::Waveform::AP Samples::getWaveform() {
	return waveform_;
      }

      const std::string& Samples::getRawData() const {
	return rawData_;
      }

      const data::Format::CP Samples::FORMAT_8_2_1 = data::Format::CP(new data::Format(8,2,1));
      const data::Format::CP Samples::FORMAT_12_2_1 = data::Format::CP(new data::Format(12,2,1));
      const data::Format::CP Samples::FORMAT_8_2_64 = data::Format::CP(new data::Format(8,2,64));
      const data::Format::CP Samples::FORMAT_8_1_128 = data::Format::CP(new data::Format(8,1,128));

    }
  }
}
