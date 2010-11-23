#ifndef BLITZORTUNG_HARDWARE_PCB_V4_H_
#define BLITZORTUNG_HARDWARE_PCB_V4_H_

#include "hardware/pcb/Base.h"
#include "data/sample/Base.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    class V4 : public Base {
      private:

	//! logger for objects of this class
	mutable Logger logger_;

	//! overloading method for local modification of gps parser
        virtual void parseGps(const std::vector<std::string>&);

      public:
      
        //! create V4 hardware object
	V4(comm::Base&, gps::Base&);

	//! delete V4 hardware object	
	virtual ~V4();

	//! returns a sample from the given splitted line returned from the measurement device
	virtual data::Sample::AP parse(const std::vector<std::string> &);
    };

  }
}
}

#endif
