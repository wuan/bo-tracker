#ifndef BLITZORTUNG_HARDWARE_PCB_V4_H_
#define BLITZORTUNG_HARDWARE_PCB_V4_H_

#include "hardware/pcb/Base.h"
#include "data/sample/Base.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    class V4 : public Base {
      private:

	//! logger for class
	mutable Logger logger_;

      public:
	V4(SerialPort&, const gps::Type&, const data::sample::Base::Creator&);
	virtual ~V4();

	virtual std::auto_ptr<data::sample::Base> parse(const std::vector<std::string> &);
    };

  }
}
}

#endif
