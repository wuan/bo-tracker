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
	os << " " << sample.getTimeDelta().total_nanoseconds();

	const Sample::Waveform& wfm = sample.getWaveform();
	for (unsigned int i=0; i < sample.getNumberOfSamples(); i++) {
	  //os.precision(3);
	  //os << " " << wfm.getTime(peak).time_of_day().total_nanoseconds() / 1e3;
	  os.precision(2);
	  os << " " << int(wfm.getX(i)) << " " << int(wfm.getY(i));
	}

	// restore original locale
	os.imbue(oldLocale);

	return os;
      }

      void Base::set(data::Sample::AP sample) {
	waveform_ = sample->releaseWaveform();

	gpsInfo_ = sample->releaseGpsInfo();
      }
      
      const Sample::Waveform& Base::getWaveform() const {
	return *waveform_;
      }

      const GpsInfo& Base::getGpsInfo() const {
	return *gpsInfo_;
      }

      float Base::getAntennaLongitude() const {
	return gpsInfo_->getLongitude();
      }

      float Base::getAntennaLatitude() const {
	return gpsInfo_->getLatitude();
      }

      short Base::getAntennaAltitude() const {
	return gpsInfo_->getAltitude();
      }

      unsigned char Base::getGpsNumberOfSatellites() const {
	return gpsInfo_->getNumberOfSatellites();
      }

      char Base::getGpsStatus() const {
	return gpsInfo_->getStatus();
      }
      
      const pt::ptime& Base::getTime() const {
	return waveform_->getTime();
      }

      const pt::time_duration& Base::getTimeDelta() const {
	return waveform_->getTimeDelta();
      }


      void Base::toStream(std::iostream& stream) const {

	// write waveform to stream
	waveform_->write(stream, getNumberOfSamples());

	// write gps information to stream
	gpsInfo_->write(stream);
      }
      
      void Base::fromStream(std::iostream& stream, const gr::date& date) {
	// read waveform from stream
	waveform_ = Sample::Waveform::AP(new Sample::Waveform(stream, date, getNumberOfSamples()));

	// read gps information from stream
	gpsInfo_ = GpsInfo::AP(new GpsInfo(stream));

      }
      
      //! get binary storage size of sample
      unsigned int Base::getSize() const {
	unsigned int gpsSize = GpsInfo::GetSize();
	
	unsigned int waveformSize = Sample::Waveform::GetSize(getNumberOfSamples());

	//std::cout << "getSize() : gps: " << gpsSize << " wfm: " << waveformSize << " , # of samples: " << getNumberOfSamples() << std::endl;
	
	return gpsSize + waveformSize;
      }
	    
      
    }
  }
}
