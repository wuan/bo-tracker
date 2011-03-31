#ifndef BLITZORTUNG_HARDWARE_GPS_DATA_TIME_H_
#define BLITZORTUNG_HARDWARE_GPS_DATA_TIME_H_

#include <boost/date_time.hpp>

#include "namespaces.h"
#include "util/RingBuffer.h"
#include "hardware/gps/data/Base.h"
#include "Logger.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	class Time : public Base {
	  private:

	    //! timestamp of actual second
	    pt::ptime second_;

	    //! counter value of actual second
	    int oldCounter_;

	    //! ring buffer for counter ticks per second
	    util::RingBuffer<int> counterTicksPerSecond_;

	    //! flag for first data value
	    int ignoreCounter_;

	    //! calculate counter difference
	    int getCounterDifference(const int counter) const;

	    //! logger for class
	    mutable Logger logger_;

	  public:

	    //! create a gps time data object
	    Time();

	    //! delete a gps time data object
	    virtual ~Time();

	    //! set actual timestamp an the corresponding counter value
	    void setSecond(const pt::ptime& dateTime, int counterValue);

	    //! get actual transmitted timestamp
	    const pt::ptime& getTime() const;

	    //! get time of event represented by counter value
	    pt::ptime getTime(const int counterValue) const;

	    //! return the actual number of counter ticks per second
	    double getTicksPerSecond() const;

	    //! returns the actual tick error in us
	    double getTickError() const;
	};

      }
    }
  }
}

#endif
