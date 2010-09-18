#ifndef BLITZORTUNG_HARDWARE_PCB_BASE_H_
#define BLITZORTUNG_HARDWARE_PCB_BASE_H_

#include <string>
#include <vector>

#include "hardware/comm/Base.h"
#include "hardware/gps/Base.h"
#include "data/sample/Base.h"

namespace blitzortung {
namespace hardware {
  namespace pcb {
   
    //! base class for blitzortung acquisition PCB
    class Base {

      public:

	typedef std::auto_ptr<Base> AP;

      private:
	//! reference to communication object
	comm::Base& comm_;

	//! vector for data fields
	std::vector<std::string> fields_;

	//! logger for objects of this class
	mutable Logger logger_;

      protected:

	//! reference to gps device object
	gps::Base& gps_;

	//! returnes int value of given hex string
	int parseHex(const std::string& hexString);

	//! sample creator;
        const data::sample::Base::Creator& sampleCreator_;	

	//! gps parser interface, overload for modification
        virtual void parseGps(const std::vector<std::string>&);

      public:

	//! constructor for base class
	Base(comm::Base&, gps::Base&, const data::sample::Base::Creator&);

	//! destructor
	virtual ~Base();

	//! returns if pcb object's connection is opened
	bool isOpen() const;

	//! returns a sample read from the hardware
	std::auto_ptr<data::sample::Base> read();

	//! returns a sample parsed from the given string vector
	/*!
	this function needs to be declared in any of the derived classes
	*/
	virtual std::auto_ptr<data::sample::Base> parse(const std::vector<std::string> &) = 0;
    };

  }
}
}

#endif
