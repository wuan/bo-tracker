#ifndef BLITZORTUNG_HARDWARE_PARSING_TICKS_H_
#define BLITZORTUNG_HARDWARE_PARSING_TICKS_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "namespaces.h"
#include "Logger.h"
#include "hardware/parsing/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      //! base class for gps hardware objects      
      class Ticks : public Base {

	private:

	  //! combined date time string
	  std::string dateTime_;

	  //! gps status character
	  std::string status_;

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

	  //! logger for this class
	  mutable Logger logger_;

	public:

	  //! constructor
	  Ticks(const std::vector<std::string>);

	  //! parse incoming data
	  bool parse(const std::vector<std::string>);

	  //! read GPS status char
	  const char getGpsStatus() const;

	  //! get longitude value
	  float getLongitude() const;

	  //! get latitude value
	  float getLatitude() const;

	  //! get altitude value
	  float getAltitude() const;

	  //! get average satellite count value
	  unsigned short getSatelliteCount() const;

	  //! get timestamp from counter value
	  const std::string& getDateTime() const;
      };

    }
  }
}
#endif
