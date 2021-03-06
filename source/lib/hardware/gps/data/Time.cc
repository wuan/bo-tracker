#include "hardware/gps/data/Time.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	Time::Time() :
	  second_(),
	  oldCounter_(),
	  counterTicksPerSecond_(Base::BUFFERSIZE),
	  ignoreCounter_(1),
	  logger_("hardware.gps.data.Time")
	{

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "initialized";
	}

	Time::~Time() {
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "deleted";
	}

	void Time::setSecond(const pt::ptime& dateTime, int counter) {
	  if (!dateTime.is_not_a_date_time()) {
	    second_ = dateTime;

	    // counter difference between calls is number of ticks per second
	    int counterTicksElapsed = getCounterDifference(counter);

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "setSecond() " << second_ << " counter difference: " << counterTicksElapsed;

	    if (ignoreCounter_ <= 0) {
	      // TODO this is a temporary fix to avoid problems introduced by bad counter values
	      if (counterTicksElapsed > 2400000 && counterTicksElapsed < 2600000) {
		counterTicksPerSecond_.add(counterTicksElapsed);
	      } else {
		logger_.warnStream() << "hardware::gps::data::Time::setSecond() counter value " << counterTicksElapsed << " out of range";
	      }
	    } else {
	      ignoreCounter_--;
	    }

	    //if (counterTicksPerSecond_.getActualSize() > 0)
	    //  std::cout << second_ << " " << counterTicksPerSecond_.getAverage() << std::endl;
	  }

	  oldCounter_ = counter;
	}

	const pt::ptime& Time::getTime() const {
	  return second_;
	}

	int Time::getCounterDifference(const int counter) const {
	  int counterDifference = counter - oldCounter_;
	  if (counterDifference < 0)
	    counterDifference += 1<<24;
	  return counterDifference;
	}

	pt::ptime Time::getTime(const int counter) const {
	  if (counterTicksPerSecond_.getActualSize() == 0) {
	    logger_.noticeStream() << "getTime() no time available";
	    return pt::not_a_date_time;
	  }

	  double counterTicksPerSecond = double(counterTicksPerSecond_.getSum()) / counterTicksPerSecond_.getActualSize();

	  int nanoseconds = 1e9 * getCounterDifference(counter) / counterTicksPerSecond;

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "getTime(): counter: " << counter << " diff " << getCounterDifference(counter) << " ns: " << nanoseconds << " ticks / s: " << counterTicksPerSecond;

	  return second_ + pt::nanoseconds(nanoseconds);
	}

	double Time::getTickError() const {
	  return counterTicksPerSecond_.getStddev() / getTicksPerSecond() * 1e6;
	}

	double Time::getTicksPerSecond() const {
	  return counterTicksPerSecond_.getAverage();
	}

      }
    }
  }
}
