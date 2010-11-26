#ifndef BLITZORTUNG_HARDWARE_PCB_V6_H_
#define BLITZORTUNG_HARDWARE_PCB_V6_H_

#include "hardware/pcb/Base.h"
#include "data/Sample.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    class V6 : public Base {

      private:

	const static pt::time_duration SAMPLE_RATE;

	//! logger for objects of this class
	mutable Logger logger_;

	//! returns a sample with the given reference time and the waveform data in the string
	data::Sample::AP parseData(const pt::ptime&, const std::string&);

      public:

        //! create V6 hardware object
	V6(comm::Base&, gps::Base&, const data::SampleFactory&);

	//! delete V6 hardware object
	virtual ~V6();

	//! returns a sample from the given splitted line returned from the measurement device
	virtual data::Sample::AP parse(const std::vector<std::string>&);
    };

  }
}
}

#endif
