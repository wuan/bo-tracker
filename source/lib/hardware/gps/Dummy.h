#ifndef BLITZORTUNG_HARDWARE_GPS_DUMMY_H_
#define BLITZORTUNG_HARDWARE_GPS_DUMMY_H_

#include <string>
#include <vector>

#include "namespaces.h"
#include "Logger.h"
#include "util/RingBuffer.h"
#include "hardware/comm/Base.h"
#include "hardware/gps/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/data/Time.h"
#include "hardware/gps/data/Location.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      //! class for access to sirf gps devices      
      class Dummy : public Base {

	private:
	
	  //! logger for objects of this class
	  mutable Logger logger_;

	  //! hardware specific method for device initialization	  
	  void initWrite(const unsigned int);

	public:

	  //! constructor
	  Dummy(comm::Base&, const unsigned short gpsBaudRate);

	  //! destructor
	  virtual ~Dummy();

	  const std::string getType() const;
      };

    }
  }
}
#endif
