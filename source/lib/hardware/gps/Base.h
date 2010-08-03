#ifndef BLITZORTUNG_HARDWARE_GPS_H_
#define BLITZORTUNG_HARDWARE_GPS_H_

#include <string>
#include <vector>

#include "namespaces.h"
#include "util/RingBuffer.h"
#include "hardware/Communication.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/data/Time.h"
#include "hardware/gps/data/Location.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      class Base {

	private:
	  //! communication object
	  Communication communication_;

	  //! gps time data object
	  data::Time time_;

	  //! gps location data object
	  data::Location location_;

	  //! gps status character
	  char status_;

	  //! ring buffer to average satellite count values
	  util::RingBuffer<int> satelliteCount_;

	  void initWrite();

	public:

	  //! constructor
	  Base(const Communication&);

	  //! destructor
	  virtual ~Base();

	  //! parse NMEA data from pcb
	  void parse(const std::vector<std::string> &fields);

	  //! set GPS status char
	  void setStatus(const char status);

	  //! read GPS status char
	  const char getStatus() const;

	  //! add actual satellite count value
	  void addSatelliteCount(const std::string &);

	  //! get average satellite count value
	  int getSatelliteCount() const;

	  //! get timestamp from counter value
	  pt::ptime getTime(const int) const;

	  //! get reference to location object
	  const data::Location& getLocation() const;

	  //! initialize GPS hardware
	  void init();
      };

    }
  }
}
#endif
