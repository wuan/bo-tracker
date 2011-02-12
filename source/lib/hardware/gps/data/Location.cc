#include "hardware/gps/data/Location.h"

#include <cmath>

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	Location::Location() :
	  longitude_(Base::BUFFERSIZE), 
	  latitude_(Base::BUFFERSIZE),
	  altitude_(Base::BUFFERSIZE)
	{
	}

	Location::~Location() {
	}

	void Location::addLongitude(const float longitude) {
	  longitude_.add(longitude);
	}

	double Location::getLongitude() const {
	  return longitude_.getAverage();
	}

	void Location::addLatitude(const float latitude) {
	  latitude_.add(latitude);
	}

	double Location::getLatitude() const {
	  return latitude_.getAverage();
	}
	  
	void Location::addAltitude(const float altitude) {
	  altitude_.add(altitude);
	}

	double Location::getAltitude() const {
	  return altitude_.getAverage();
	}

      }
    }
  }
}
