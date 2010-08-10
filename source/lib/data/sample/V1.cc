#include "data/sample/V1.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      V1::V1() {
	//! allocate data
	data_ = new(char[getDataSize()]);
      }

      V1::~V1() {
      }

      Base* V1::Creator::operator()() const {
	return new V1();
      }

      const unsigned int V1::IDX_TIME = 0;
      const unsigned int V1::IDX_LONGITUDE = IDX_TIME + sizeof(TYPE_TIME);
      const unsigned int V1::IDX_LATITUDE = IDX_LONGITUDE + sizeof(TYPE_COORDINATE);
      const unsigned int V1::IDX_GPSNUMBEROFSATELLITES = IDX_LATITUDE + sizeof(TYPE_COORDINATE);
      const unsigned int V1::IDX_GPSSTATUS = IDX_GPSNUMBEROFSATELLITES + sizeof(TYPE_GPSNUMBEROFSATELLITES);
      const unsigned int V1::IDX_OFFSET1 = IDX_GPSSTATUS + sizeof(TYPE_GPSSTATUS);
      const unsigned int V1::IDX_XAMP1 = IDX_OFFSET1 + sizeof(TYPE_OFFSET);
      const unsigned int V1::IDX_YAMP1 = IDX_XAMP1 + sizeof(TYPE_AMPLITUDE);
      const unsigned int V1::DATASIZE = IDX_YAMP1 + sizeof(TYPE_AMPLITUDE);

      pt::time_duration V1::getTime(int index) const {

	TYPE_TIME total_nanoseconds = *(TYPE_TIME*)&data_[IDX_TIME];

	static const long long int nano_factor = 1000000000;
	int seconds = total_nanoseconds / nano_factor;
	long long int nanoseconds = total_nanoseconds - seconds * nano_factor;

	switch (index) {
	  case 0:
	    break;

	  case 1:
	    nanoseconds += 3125 * *(TYPE_OFFSET*) &data_[IDX_OFFSET1];
	    break;

	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::getTime() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}

	return pt::seconds(seconds) + pt::nanoseconds(nanoseconds);
      }

      void V1::setTime(const pt::time_duration& time) {
	*(TYPE_TIME*)&data_[IDX_TIME] = time.total_nanoseconds();
      }

      void V1::setOffset(unsigned short offsetfactor, int index) {
	switch (index) {
	  case 1:
	    *(TYPE_OFFSET*)&data_[IDX_OFFSET1] = offsetfactor;
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::setOffset() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      void V1::setAmplitude(const short ampx, const short ampy, int index) {
	switch (index) {
	  case 1:
	    *(TYPE_AMPLITUDE*)&data_[IDX_XAMP1] = ampx;
	    *(TYPE_AMPLITUDE*)&data_[IDX_YAMP1] = ampy;
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::setOffset() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      short V1::getXAmplitude(int index) const {
	switch (index) {
	  case 1:
	    return *(TYPE_AMPLITUDE*)&data_[IDX_XAMP1];
	    break;
	  default:
	    std::ostringstream oss;
	    oss << "data::sample::V1::setOffset() invalid index " << index << " used";
	    throw exception::Base(oss.str());
	}
      }

      short V1::getYAmplitude(int index) const {
	return *(TYPE_AMPLITUDE*)&data_[IDX_YAMP1];
      }

      void V1::setAntennaLongitude(const float longitude) {
	*(TYPE_COORDINATE*)&data_[IDX_LONGITUDE] = longitude;
      }

      float V1::getAntennaLongitude() const {
	return *(TYPE_COORDINATE*)&data_[IDX_LONGITUDE];
      }

      void V1::setAntennaLatitude(const float latitude) {
	*(TYPE_COORDINATE*)&data_[IDX_LATITUDE] = latitude;
      }

      float V1::getAntennaLatitude() const {
	return *(TYPE_COORDINATE*)&data_[IDX_LATITUDE];
      }

      void V1::setGpsNumberOfSatellites(const unsigned char numberOfSatellites) {
	*(TYPE_GPSNUMBEROFSATELLITES*)&data_[IDX_GPSNUMBEROFSATELLITES] = numberOfSatellites;
      }

      unsigned char V1::getGpsNumberOfSatellites() const {
	return *(TYPE_GPSNUMBEROFSATELLITES*)&data_[IDX_GPSNUMBEROFSATELLITES];
      }

      void V1::setGpsStatus(const char gpsStatus) {
	*(TYPE_GPSSTATUS*)&data_[IDX_GPSSTATUS] = gpsStatus;
      }

      char V1::getGpsStatus() const {
	return *(TYPE_GPSSTATUS*)&data_[IDX_GPSSTATUS];
      }

      unsigned int V1::getDataSize() const {
	return DATASIZE;
      }

    }
  }
}
