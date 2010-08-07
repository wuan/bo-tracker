#include "hardware/gps/data/Time.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	Time::Time() :
	  counterTicksPerSecond_(Base::BUFFERSIZE),
	  ignoreCounter_(2)
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

	  // counter difference between calls is number of ticks per second
	  int counterTicksElapsed = getCounterDifference(counter);

	  if (ignoreCounter_ <= 0)
	    counterTicksPerSecond_.add(counterTicksElapsed);
	  else
	    ignoreCounter_--;

	  //if (counterTicksPerSecond_.getActualSize() > 0)
	  //  std::cout << second_ << " " << counterTicksPerSecond_.getAverage() << std::endl;

	  oldCounter_ = counter;
	}

	const pt::ptime& Time::getSecond() const {
	  return second_;
	}

	int Time::getCounterDifference(const int counter) const {
	  int counterDifference = counter - oldCounter_;
	  if (counterDifference < 0)
	    counterDifference += 1<<24;
	  return counterDifference;
	}

	pt::ptime Time::getTime(const int counter) const {
	  double counterTicksPerSecond = double(counterTicksPerSecond_.getSum()) / counterTicksPerSecond_.getActualSize();

	  int nanoseconds = 1e9 * getCounterDifference(counter) / counterTicksPerSecond;

	  return second_ + pt::nanoseconds(nanoseconds);
	}

      }
    }
  }
}
