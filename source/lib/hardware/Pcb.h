#ifndef BLITZORTUNG_HARDWARE_PCB_H_
#define BLITZORTUNG_HARDWARE_PCB_H_

#include <string>
#include <vector>

#include "hardware/comm/Base.h"
#include "hardware/gps/Base.h"
#include "data/Event.h"
#include "data/Format.h"
#include "hardware/parsing/Samples.h"

namespace blitzortung {
  namespace hardware {

    //! base class for blitzortung acquisition PCB
    class Pcb {

      public:

	typedef std::auto_ptr<Pcb> AP;

      private:
	//! reference to communication object
	comm::Base& comm_;

	//! vector for data fields
	std::vector<std::string> fields_;

      protected:

	//! reference to gps device object
	gps::Base& gps_;

      private:

	//! firmware version of controller
	unsigned short firmwareVersion_;

	//! logger for objects of this class
	mutable Logger logger_;

      public:

	//! constructor for base class
	Pcb(comm::Base&, gps::Base&, unsigned short firmwareVersion=0);

	//! destructor
	virtual ~Pcb();

	//! returns if pcb object's connection is opened
	bool isOpen() const;

	//! returns a sample read from the hardware
	data::Event::AP read();

	//! returns a sample created from the given parser data
	data::Event::AP createSample(parsing::Samples& samplesParser);
    };

  }
}

#endif
