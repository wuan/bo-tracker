#ifndef BLITZORTUNG_HARDWARE_GPS_DATA_TIME_H_
#define BLITZORTUNG_HARDWARE_GPS_DATA_TIME_H_

#include <boost/date_time.hpp>

#include "namespaces.h"
#include "util/RingBuffer.h"
#include "hardware/gps/data/Base.h"

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
	    int counter_;

	    //! ring buffer for timer ticks per second
	    util::RingBuffer<int> timerSecond_;

	    //! flag for first data value
	    bool firstValue_;

	    //! calculate counter difference
	    int getCounterDifference(const int counter) const;

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
