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

	double Location::getCoordFromNmea(const std::string &nmeacoord) {
	  double coord;
	  std::istringstream coordstring(nmeacoord);
	  coordstring >> coord;

	  double degrees = floor(coord / 100.0);

	  return degrees + (coord - degrees * 100.0) / 60.0;
	}

	void Location::addLongitude(const std::string &longitudeString, const char hemisphere) {
	  double longitude = getCoordFromNmea(longitudeString);

	  if (hemisphere == 'W')
	    longitude *= -1.0;

	  longitude_.add(longitude);
	}

	double Location::getLongitude() const {
	  return longitude_.getAverage();
	}

	void Location::addLatitude(const std::string &latitudeString, const char hemisphere) {
	  double latitude = getCoordFromNmea(latitudeString);

	  if (hemisphere == 'S')
	    latitude *= -1.0;

	  latitude_.add(latitude);
	}

	double Location::getLatitude() const {
	  return latitude_.getAverage();
	}
	  
	void Location::addAltitude(const std::string &altitudeString) {
	  double altitude;
	  std::istringstream altitudeStream(altitudeString);
	  altitudeStream >> altitude;

	  altitude_.add(altitude);
	}

	double Location::getAltitude() const {
	  return altitude_.getAverage();
	}





      }
    }
  }
}
