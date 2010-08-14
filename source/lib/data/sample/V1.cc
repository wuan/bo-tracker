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

      pt::time_duration V1::getOffset(int index) const {
	switch (index) {
	  case 1:
	    return pt::nanoseconds(3125 * peak1Offset_);
	    break;

	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::getTime() invalid peak index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      pt::ptime V1::getTime(int index) const {

	if (index > 0)
	  return time_ + getOffset(index);
	else
	  return time_;
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

      unsigned short V1::getVersion() const {
	return 1;
      }

      void V1::toStream(std::iostream& stream) const {
	long long int nanoseconds = getTime().time_of_day().total_nanoseconds();
	valueToStream(stream, nanoseconds);
	valueToStream(stream, longitude_);
	valueToStream(stream, latitude_);
	valueToStream(stream, altitude_);
	valueToStream(stream, gpsNumberOfSatellites_);
	valueToStream(stream, gpsStatus_);
	valueToStream(stream, peak1Offset_);
	valueToStream(stream, peak1A_);
	valueToStream(stream, peak1B_);
      }
      
      void V1::fromStream(std::iostream& stream, const gr::date& fileDate) {
	long long int nanoseconds;
	valueFromStream(stream, nanoseconds);
	time_ = pt::ptime(fileDate, pt::nanoseconds(nanoseconds));
	valueFromStream(stream, longitude_);
	valueFromStream(stream, latitude_);
	valueFromStream(stream, altitude_);
	valueFromStream(stream, gpsNumberOfSatellites_);
	valueFromStream(stream, gpsStatus_);
	valueFromStream(stream, peak1Offset_);
	valueFromStream(stream, peak1A_);
	valueFromStream(stream, peak1B_);
      }

      unsigned int V1::getSize() const {
	Size size;

	long long int nanoseconds;
	size.add(nanoseconds);
	size.add(longitude_);
	size.add(latitude_);
	size.add(altitude_);
	size.add(gpsNumberOfSatellites_);
	size.add(gpsStatus_);
	size.add(peak1Offset_);
	size.add(peak1A_);
	size.add(peak1B_);

	return size.get();
      }


    }
  }
}
