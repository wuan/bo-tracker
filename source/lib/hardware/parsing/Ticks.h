#ifndef BLITZORTUNG_HARDWARE_PARSING_TICKS_H_
#define BLITZORTUNG_HARDWARE_PARSING_TICKS_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "namespaces.h"
#include "Logger.h"
#include "data/GpsInfo.h"
#include "util/RingBuffer.h"
#include "hardware/gps/data/Time.h"
#include "hardware/gps/data/Location.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      //! base class for gps hardware objects      
      class Ticks {

	private:

	  //! combined date time string
	  std::string dateTime_;

	  //! counter vaoue
	  unsigned int counter_; 

	  //! gps status character
	  char status_;

	  //! gps longitude
	  float longitude_;

	  //! gps latitude
	  float latitude_;

	  //! gps altitude
	  float altitude_;

	  //! gps number of satellites in use
	  unsigned short satelliteCount_;

	  //! firmware version of the microcontroller
	  std::string firmwareVersion_;

	public:

	  //! constructor
	  Ticks(const std::vector<std::string>);

	  //! destructor
	  virtual ~Ticks();

	  //! parse incoming data
	  bool parse(const std::vector<std::string>);

	  //! read GPS status char
	  const char getStatus() const;

	  //! is GPS data valid?
	  const bool isValid() const;

	  //! get average satellite count value
	  int getSatelliteCount() const;

	  //! get timestamp from counter value
	  const std::string getTime() const;
      };

    }
  }
}
#endif
