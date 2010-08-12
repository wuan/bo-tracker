#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      V1::V1() {
      }

      V1::~V1() {
      }

      Base* V1::Creator::operator()() const {
	return new V1();
      }


      pt::ptime V1::getTime(int index) const {

	pt::time_duration timeOffset(pt::seconds(0));

	switch (index) {
	  case 0:
	    break;

	  case 1:
	    timeOffset = pt::nanoseconds(3125 * peak1Offset_);
	    break;

	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::getTime() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}

	return time_ + timeOffset;
      }

      void V1::setTime(const pt::ptime& time) {
	time_ = time;
      }

      void V1::setOffset(short offsetfactor, int index) {

	switch (index) {
	  case 1:
	    peak1Offset_ = offsetfactor;
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::setOffset() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      void V1::setAmplitude(const float ampx, const float ampy, int index) {
	switch (index) {
	  case 1:
	    peak1A_ = ampx;
	    peak1B_ = ampy;
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::setOffset() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      float V1::getXAmplitude(int index) const {
	switch (index) {
	  case 1:
	    return peak1A_;
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::setXAmplitude() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      float V1::getYAmplitude(int index) const {
	switch (index) {
	  case 1:
	    return peak1B_;
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::getYAmplitude() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      void V1::setAntennaLongitude(const float longitude) {
	longitude_ = longitude;
      }

      float V1::getAntennaLongitude() const {
	return longitude_;
      }

      void V1::setAntennaLatitude(const float latitude) {
	latitude_ = latitude;
      }

      float V1::getAntennaLatitude() const {
	return latitude_;
      }

      void V1::setAntennaAltitude(const short altitude) {
	altitude_ = altitude;
      }

      short V1::getAntennaAltitude() const {
	return altitude_;
      }

      void V1::setGpsNumberOfSatellites(const unsigned char gpsNumberOfSatellites) {
	gpsNumberOfSatellites_ = gpsNumberOfSatellites;
      }

      unsigned char V1::getGpsNumberOfSatellites() const {
	return gpsNumberOfSatellites_;
      }

      void V1::setGpsStatus(const char gpsStatus) {
	gpsStatus_ = gpsStatus;
      }

      char V1::getGpsStatus() const {
	return gpsStatus_;
      }

    }
  }
}
