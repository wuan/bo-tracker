#ifndef BLITZORTUNG_HARDWARE_PCB_V6_H_
#define BLITZORTUNG_HARDWARE_PCB_V6_H_

#include "hardware/pcb/Base.h"
#include "data/sample/Version1.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    class V6 : public Base {
      private:

	std::auto_ptr<data::sample::Base> parseData(const pt::ptime&, const std::string &);
	short parseHex(const std::string &);

      public:
	V6(SerialPort&);
	virtual ~V6();

	std::auto_ptr<data::sample::Base> parse(const std::vector<std::string> &);
    };

  }
}
}

#endif
