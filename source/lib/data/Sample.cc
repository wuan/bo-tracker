#include "data/Sample.h"

namespace blitzortung {
  namespace data {

    Sample::Sample(Waveform::AP waveform, data::GpsInfo::AP gpsInfo) :
      waveform_(waveform),
      gpsInfo_(gpsInfo)
    {
    }

    Sample::~Sample()
    {
    }

    const Sample::Waveform& Sample::getWaveform() const {
      return *waveform_;
    }

    Sample::Waveform::AP Sample::releaseWaveform() {
      return waveform_;
    }

    const GpsInfo& Sample::getGpsInfo() const {
      return *gpsInfo_;
    }

    GpsInfo::AP Sample::releaseGpsInfo() {
      return gpsInfo_;
    }

    bool Sample::operator<(const Sample &rhs) const {
      return waveform_->getTime() < rhs.waveform_->getTime();
    } 

    bool Sample::CompareAmplitude::operator()(const first_argument_type &x, const second_argument_type &y) const {
      return x.getWaveform().getAmplitude(0) < y.getWaveform().getAmplitude(0);
    } 

    std::ostream& operator <<(std::ostream& os, const bo::data::Sample &sample) {
      const data::Sample::Waveform& wfm = sample.getWaveform();
      const data::GpsInfo& gpsInfo = sample.getGpsInfo();

      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S.%f");
      std::locale oldLocale = os.imbue(std::locale(std::locale::classic(), timefacet));

      os.setf(std::ios::fixed);
      os.precision(4);
      os << wfm.getTime() << " " << gpsInfo.getLongitude() << " " << gpsInfo.getLatitude();
      os << " " << gpsInfo.getAltitude();
      os << " " << (int) gpsInfo.getNumberOfSatellites();
      os << " " << wfm.getTimeDelta().total_nanoseconds();

      for (unsigned int i=0; i < wfm.getNumberOfSamples(); i++) {
	//os.precision(3);
	//os << " " << wfm.getTime(peak).time_of_day().total_nanoseconds() / 1e3;
	os.precision(2);
	os << " " << int(wfm.getX(i)) << " " << int(wfm.getY(i));
      }

      // restore original locale
      os.imbue(oldLocale);

      return os;
    }


  }
}
