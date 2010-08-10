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
	    //! stringstream used for date time parsing
	    std::istringstream dateTimeInput_;

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
	    Time();
	    virtual ~Time();

	    void setSecond(const std::string &timeString, int counter);

	    const pt::ptime& getSecond() const;

	    pt::ptime getTime(const int) const;
	};

      }
    }
  }
}

#endif
