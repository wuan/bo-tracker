#include "data/sample/Base.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      Base::Base() {
      }

      Base::~Base() {
      }


      bool Base::operator<(const Base &rhs) const {
	return getTime() < rhs.getTime();
      } 

      bool Base::CompareAmplitude::operator()(const first_argument_type &x, const second_argument_type &y) const {
	return x.getWaveform().getAmplitude(0) < y.getWaveform().getAmplitude(0);
      } 

      std::ostream& operator <<(std::ostream& os, const bo::data::sample::Base &sample) {

	pt::time_facet *timefacet = new pt::time_facet();
	timefacet->format("%Y-%m-%d %H:%M:%S.%f");
	std::locale oldLocale = os.imbue(std::locale(std::locale::classic(), timefacet));

	os.setf(std::ios::fixed);
	os.precision(4);
	os << sample.getTime() << " " << sample.getAntennaLongitude() << " " << sample.getAntennaLatitude();
	os << " " << sample.getAntennaAltitude();
	os << " " << (int) sample.getGpsNumberOfSatellites();

	for (unsigned int i=0; i < sample.getNumberOfSamples(); i++) {
	  const Sample::Waveform& wfm = sample.getWaveform();
	  //os.precision(3);
	  //os << " " << wfm.getTime(peak).time_of_day().total_nanoseconds() / 1e3;
	  os.precision(2);
	  os << " " << wfm.getX(i) << " " << wfm.getY(i);
	}

	// restore original locale
	os.imbue(oldLocale);

	return os;
      }

      void Base::set(data::Sample::AP sample) {
	waveform_ = sample->releaseWaveform();

	GpsInfo::AP gpsInfo = sample->releaseGpsInfo();

	longitude_ = gpsInfo->getLongitude();
	latitude_ = gpsInfo->getLatitude();
	altitude_ = gpsInfo->getAltitude();
	gpsNumberOfSatellites_ = gpsInfo->getSatelliteCount();
	gpsStatus_ = gpsInfo->getStatus();
	
      }
      
      const Sample::Waveform& Base::getWaveform() const {
	return *waveform_;
      }

      float Base::getAntennaLongitude() const {
	return longitude_;
      }

      float Base::getAntennaLatitude() const {
	return latitude_;
      }

      short Base::getAntennaAltitude() const {
	return altitude_;
      }

      unsigned char Base::getGpsNumberOfSatellites() const {
	return gpsNumberOfSatellites_;
      }

      char Base::getGpsStatus() const {
	return gpsStatus_;
      }
      
      const pt::ptime& Base::getTime() const {
	return waveform_->getTime();
      }

      void Base::toStream(std::iostream& stream) const {

	// write waveform to stream
	waveform_->write(stream, getNumberOfSamples());

	// write gps information to stream
	util::Stream::WriteValue(stream, longitude_);
	util::Stream::WriteValue(stream, latitude_);
	util::Stream::WriteValue(stream, altitude_);
	util::Stream::WriteValue(stream, gpsNumberOfSatellites_);
	util::Stream::WriteValue(stream, gpsStatus_);
      }
      
      void Base::fromStream(std::iostream& stream, const gr::date& date) {
	// read waveform from stream
	waveform_ = Sample::Waveform::AP(new Sample::Waveform(stream, date, getNumberOfSamples()));

	// read gps information from stream
	util::Stream::ReadValue(stream, longitude_);
	util::Stream::ReadValue(stream, latitude_);
	util::Stream::ReadValue(stream, altitude_);
	util::Stream::ReadValue(stream, gpsNumberOfSatellites_);
	util::Stream::ReadValue(stream, gpsStatus_);
      }
      
      //! get binary storage size of sample
      unsigned int Base::getSize() const {
	util::Size size;
	size.add(longitude_);
	size.add(latitude_);
	size.add(altitude_);
	size.add(gpsNumberOfSatellites_);
	size.add(gpsStatus_);
	
	unsigned int waveformSize = Sample::Waveform::GetSize(getNumberOfSamples());
	
	return size.get() + waveformSize;
      }
	    
      
    }
  }
}
