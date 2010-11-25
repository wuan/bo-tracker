#include "data/GpsInfo.h"
#include "hardware/gps/Base.h"
#include "hardware/gps/data/Location.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    GpsInfo::GpsInfo(const hardware::gps::Base& gps)
    {
      const hardware::gps::data::Location& location = gps.getLocation();

      longitude_ = location.getLongitude();
      latitude_ = location.getLatitude();
      altitude_ = location.getAltitude();
      
      numberOfSatellites_ = gps.getSatelliteCount();
      status_ = gps.getStatus();
    }

    GpsInfo::GpsInfo(std::iostream& stream) {
	util::Stream::ReadValue(stream, longitude_);
	util::Stream::ReadValue(stream, latitude_);
	util::Stream::ReadValue(stream, altitude_);
	util::Stream::ReadValue(stream, numberOfSatellites_);
	util::Stream::ReadValue(stream, status_);
    }

    GpsInfo::GpsInfo()
    {
      longitude_ = 11.2;
      latitude_ = 48.1;
      altitude_ = 535;
      
      numberOfSatellites_ = 6;
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

    short GpsInfo::getAltitude() const {
      return altitude_;
    }

    unsigned char GpsInfo::getNumberOfSatellites() const {
      return numberOfSatellites_;
    }

    char GpsInfo::getStatus() const {
      return status_;
    }

    void GpsInfo::write(std::iostream& stream) {
      // write gps information to stream
      util::Stream::WriteValue(stream, longitude_);
      util::Stream::WriteValue(stream, latitude_);
      util::Stream::WriteValue(stream, altitude_);
      util::Stream::WriteValue(stream, numberOfSatellites_);
      util::Stream::WriteValue(stream, status_);
    }

    unsigned int GpsInfo::getSize() {

      util::Size size;
      size.add(longitude_);
      size.add(latitude_);
      size.add(altitude_);
      size.add(numberOfSatellites_);
      size.add(status_);

      return size.get();
    }

    unsigned int GpsInfo::GetSize() {
      static GpsInfo gpsInfo;

      return gpsInfo.getSize();
    }

  }
}
