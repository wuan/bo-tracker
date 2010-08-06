#include "hardware/gps/data/Time.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	Time::Time() :
	  timerSecond_(Base::BUFFERSIZE),
	  firstValue_(true)
	{
	  pt::time_input_facet *facet = new pt::time_input_facet();
	  facet->format("%d%m%y %H%M%S");
	  dateTimeInput_.imbue(std::locale(std::locale::classic(), facet));
	}

	Time::~Time() {
	}

	void Time::setSecond(const std::string &timeString, int counter) {
	  dateTimeInput_.str(timeString);
	  dateTimeInput_ >> second_;

	  // TODO this is only a temporary fix
	  second_ += pt::seconds(1);

	  int counterDifference = getCounterDifference(counter);

	  if (firstValue_) {
	    firstValue_ = false;
	  } else {
	    timerSecond_.add(counterDifference);
	  }

	  counter_ = counter;
	}

	const pt::ptime& Time::getSecond() const {
	  return second_;
	}

	int Time::getCounterDifference(const int counter) const {
	  int counterDifference = counter - counter_;
	  if (counterDifference < 0)
	    counterDifference += 0x1000000;
	  return counterDifference;
	}

	pt::ptime Time::getTime(const int counter) const {
	  int nanoseconds = 1e9 * getCounterDifference(counter) / timerSecond_.getAverage();
	  return second_ + pt::nanoseconds(nanoseconds);
	}

      }
    }
  }
}
