#ifndef BLITZORTUNG_HARDWARE_GPS_BASE_H_
#define BLITZORTUNG_HARDWARE_GPS_BASE_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "namespaces.h"
#include "Logger.h"
#include "util/RingBuffer.h"
#include "hardware/comm/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/data/Time.h"
#include "hardware/gps/data/Location.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      class Base {

	public:

	  typedef std::auto_ptr<Base> AP;

	protected:

	  //! communication object
	  comm::Base& communication_;

	private:

	  //! gps time data object
	  data::Time time_;

	  //! gps location data object
	  data::Location location_;

	  //! gps status character
	  char status_;

	  //! date when gps was last initialized
	  gr::date dateInitialized_;

	  //! ring buffer to average satellite count values
	  util::RingBuffer<int> satelliteCount_;

	  //! logger for class
	  mutable Logger logger_;

	  virtual void initWrite(const unsigned int) = 0;

	public:

	  //! constructor
	  Base(comm::Base&);

	  //! destructor
	  virtual ~Base();

	  //! parse NMEA data from pcb
	  void parse(const std::vector<std::string> &fields);

	  //! set GPS status char
	  void setStatus(const char status);

	  //! read GPS status char
	  const char getStatus() const;

	  //! is GPS data valid?
	  const bool isValid() const;

	  //! add actual satellite count value
	  void addSatelliteCount(const std::string &);

	  //! get average satellite count value
	  int getSatelliteCount() const;

	  //! get timestamp from counter value
	  pt::ptime getTime(const int) const;

	  //! get reference to location object
	  const data::Location& getLocation() const;

	  //! initialize GPS hardware
	  void init(bool force=false);
      };

    }
  }
}
#endif
