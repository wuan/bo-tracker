#ifndef BLITZORTUNG_HARDWARE_PCB_BASE_H_
#define BLITZORTUNG_HARDWARE_PCB_BASE_H_

#include <string>
#include <vector>

#include "hardware/Communication.h"
#include "hardware/gps/Base.h"
#include "data/sample/Base.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    class Base {
      private:
	//! serial port object
	Communication communication_;

	//! data field output
	std::vector<std::string> fields_;

	//! logger reference
	mutable Logger logger_;

      protected:
	//! gps device object
	gps::Base gps_;

	//! function for parsing of hex strings
	int parseHex(const std::string& hexString);

	//! sample creator;
        const data::sample::Base::Creator& sampleCreator_;	

      public:
	Base(SerialPort&, const gps::Type&, const data::sample::Base::Creator&);
	virtual ~Base();

	bool isOpen() const;

	std::auto_ptr<data::sample::Base> read();

	virtual std::auto_ptr<data::sample::Base> parse(const std::vector<std::string> &) =0;
    };

  }
}
}

#endif
