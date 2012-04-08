#ifndef BLITZORTUNG_HARDWARE_PARSING_SAMPLES_H_
#define BLITZORTUNG_HARDWARE_PARSING_SAMPLES_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "namespaces.h"
#include "Logger.h"
#include "hardware/parsing/Base.h"
#include "hardware/gps/Base.h"
#include "data/Waveform.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      //! base class for gps hardware objects      
      class Samples : public Base {

	private:

	  static const data::Format FORMAT_8_2_1;
	  static const data::Format FORMAT_12_2_1;
	  static const data::Format FORMAT_8_2_64;
	  static const data::Format FORMAT_8_1_128;
	  static const data::Format FORMAT_8_2_256;

	  //! combined date time string
	  data::Waveform::AP waveform_;

	  //! string containing the raw data transmitted from the hardware
	  std::string rawData_;

	  //! bits per data sample
	  unsigned char bitsPerSample_;

	  //! logger for this class
	  mutable Logger logger_;

	  //! set parsed waveform
	  void setWaveform(const data::Format&, const pt::ptime&, const pt::time_duration&&, const std::string&&, unsigned char bitsPerSample=0);

	public:

	  //! constructor
	  Samples(const std::vector<std::string>& fields, const hardware::gps::Base& gps);

	  //! get raw sample data string
	  const std::string& getRawData() const;

	  //! get parsed waveform
	  data::Waveform::AP getWaveform();
      };

    }
  }
}
#endif
