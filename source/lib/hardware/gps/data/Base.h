#ifndef BLITZORTUNG_HARDWARE_GPS_DATA_BASE_H_
#define BLITZORTUNG_HARDWARE_GPS_DATA_BASE_H_

#include "namespaces.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	class Base {

	  public:
	    static const int BUFFERSIZE;

	    virtual ~Base() {};

	};

      }
    }
  }
}
#endif
