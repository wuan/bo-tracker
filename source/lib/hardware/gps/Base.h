#ifndef BLITZORTUNG_HARDWARE_GPS_BASE_H_
#define BLITZORTUNG_HARDWARE_GPS_BASE_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "namespaces.h"
#include "Logger.h"
#include "data/GpsInfo.h"
#include "util/RingBuffer.h"
#include "hardware/comm/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/data/Time.h"
#include "hardware/gps/data/Location.h"
#include "hardware/parsing/Ticks.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      //! base class for gps hardware objects      
      class Base {

	public:

	  //! typedef of auto pointer to class objects
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
	  util::RingBuffer<unsigned short> satelliteCount_;

	  //! logger for class
	  mutable Logger logger_;

          //! hardware specific method for device initialization to be overloaded	  
	  virtual void initWrite(const unsigned int) = 0;

	public:

	  //! constructor
	  Base(comm::Base&);

	  //! destructor
	  virtual ~Base();

	  //! parse read parsed date
	  void set(const hardware::parsing::Ticks& ticksParser);

	  //! set GPS status char
	  void setStatus(const char status);

	  //! read GPS status char
	  const char getStatus() const;

	  //! is GPS data valid?
	  const bool isValid() const;

	  //! add actual satellite count value
	  void addSatelliteCount(const unsigned short);

	  //! get average satellite count value
	  unsigned short getSatelliteCount() const;

	  //! get timestamp from counter value
	  pt::ptime getTime(const int) const;

	  //! get reference to location object
	  const data::Location& getLocation() const;

	  //! initialize GPS hardware
	  void init(bool force=false);

	  //! get actual gps information
	  blitzortung::data::GpsInfo::AP getInfo() const;

	  //! get gps device type
	  virtual const std::string getType() const = 0;
      };

    }
  }
}
#endif
