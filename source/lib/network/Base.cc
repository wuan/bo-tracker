#include "network/Base.h"

namespace blitzortung {
  namespace network {

    Base::Base() {
    }

    Base::~Base() {
    }


    void Base::put(std::auto_ptr<data::sample::Base> sample) {

      std::cout << sample->getTime() << " " << sample->getAntennaLongitude() << " " << sample->getAntennaLatitude() << " " << (int) sample->getGpsNumberOfSatellites() << std::endl;
      for (int peak=1; peak<=1; peak++) {
	std::cout << "  " << peak << " " << sample->getTime(peak) << " " << sample->getAmplitude(peak) << " (" << sample->getXAmplitude(peak) << ", " << sample->getYAmplitude(peak) << ")" << std::endl;
      }
    }

  }
}

