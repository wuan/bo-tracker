#include "hardware/gps/data/Time.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	Time::Time() :
	  counterTicksPerSecond_(Base::BUFFERSIZE),
	  ignoreCounter_(2),
	  logger_(Logger::get())
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

	  //std::cout << "  " << second_ << " counterDiff " << counterTicksElapsed << std::endl;

	  if (ignoreCounter_ <= 0) {
	    // TODO this is a temporary fix to avoid problems introduce by bad counter values
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
	  if (counterTicksPerSecond_.getActualSize() == 0)
	    return pt::not_a_date_time;

	  double counterTicksPerSecond = double(counterTicksPerSecond_.getSum()) / counterTicksPerSecond_.getActualSize();

	  int nanoseconds = 1e9 * getCounterDifference(counter) / counterTicksPerSecond;

	  //std::cout << "counter: " << counter << " diff " << getCounterDifference(counter) << " ns: " << nanoseconds << " ticks / s: " << counterTicksPerSecond << std::endl;

	  return second_ + pt::nanoseconds(nanoseconds);
	}

      }
    }
  }
}
