#ifndef BLITZORTUNG_HARDWARE_GPS_SIRF_H_
#define BLITZORTUNG_HARDWARE_GPS_SIRF_H_

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

      class Sirf : public Base {

	private:
	  //! logger for class
	  mutable Logger logger_;

	  void initWrite(const unsigned int);

	public:

	  //! constructor
	  Sirf(comm::Base&);

	  //! destructor
	  virtual ~Sirf();

      };

    }
  }
}
#endif
