#ifndef BLITZORTUNG_HARDWARE_PCB_V6_H_
#define BLITZORTUNG_HARDWARE_PCB_V6_H_

#include "hardware/pcb/Base.h"
#include "data/sample/Base.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    class V6 : public Base {
      private:

	//! logger for class
	mutable Logger logger_;

	std::auto_ptr<data::sample::Base> parseData(const pt::ptime&, const std::string &);

      public:

	V6(comm::Base&, gps::Base&, const data::sample::Base::Creator&);

	virtual ~V6();

	virtual std::auto_ptr<data::sample::Base> parse(const std::vector<std::string> &);
    };

  }
}
}

#endif
