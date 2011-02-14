#ifndef BLITZORTUNG_HARDWARE_PARSING_DATA_H_
#define BLITZORTUNG_HARDWARE_PARSING_DATA_H_

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

	  //! combined date time string
	  data::Waveform::AP waveform_;

	  //! string containing the raw data transmitted from the hardware
	  std::string rawData_;

	  //! logger for this class
	  mutable Logger logger_;

	public:

	  //! constructor
	  Samples(const std::vector<std::string>, const hardware::gps::Base& gps);

	  //! get timestamp from counter value
	  const std::string& getRawData() const;

	  //! get parsed waveform
	  data::Waveform::AP getWaveform();
      };

    }
  }
}
#endif
