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

  }
}
