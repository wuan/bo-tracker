#include "data/GpsInfo.h"
#include "hardware/gps/Base.h"
#include "hardware/gps/data/Location.h"
#include "util/Stream.h"

namespace blitzortung {
  namespace data {

    GpsInfo::GpsInfo(const hardware::gps::Base& gps)
    {
      const hardware::gps::data::Location& location = gps.getLocation();

      longitude_ = location.getLongitude();
      latitude_ = location.getLatitude();
      altitude_ = location.getAltitude();
      
      satelliteCount_ = gps.getSatelliteCount();
      status_ = gps.getStatus();
    }

    GpsInfo::GpsInfo()
    {
      longitude_ = 11.2;
      latitude_ = 48.1;
      altitude_ = 535;
      
      satelliteCount_ = 6;
      status_ = 'A';
    }

    GpsInfo::~GpsInfo()
    {
    }

    float GpsInfo::getLongitude() const {
      return longitude_;
    }

    float GpsInfo::getLatitude() const {
      return latitude_;
    }

    float GpsInfo::getAltitude() const {
      return altitude_;
    }

    unsigned char GpsInfo::getSatelliteCount() const {
      return satelliteCount_;
    }

    char GpsInfo::getStatus() const {
      return status_;
    }

  }
}
