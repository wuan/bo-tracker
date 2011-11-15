#include "data/Event.h"

namespace blitzortung {
  namespace data {

    Event::Event(Waveform::AP&& waveform, GpsInfo::AP&& gpsInfo)
    {
      waveform_ = std::move(waveform);
      gpsInfo_ = std::move(gpsInfo);
    }

    Event::Event(data::Format::CP format, const gr::date& date, std::iostream& stream) {
      waveform_ = Waveform::AP(new Waveform(format, date, stream));
      gpsInfo_ = GpsInfo::AP(new GpsInfo(stream));
    }

    Event::~Event() {
    }

    const Waveform& Event::getWaveform() const {
      return *waveform_;
    }

    Waveform::AP Event::releaseWaveform() {
      return std::move(waveform_);
    }

    const GpsInfo& Event::getGpsInfo() const {
      return *gpsInfo_;
    }

    GpsInfo::AP Event::releaseGpsInfo() {
      return std::move(gpsInfo_);
    }

    bool Event::operator<(const Event &rhs) const {
      return waveform_->getTime() < rhs.waveform_->getTime();
    } 

    bool Event::CompareAmplitude::operator()(const first_argument_type &x, const second_argument_type &y) const {
      return x.getWaveform().getAmplitude(0) < y.getWaveform().getAmplitude(0);
    } 
    
    void Event::toStream(std::iostream& stream) const {

      // write waveform to stream
      waveform_->toStream(stream);

      // write gps information to stream
      gpsInfo_->toStream(stream);
    }

    //! get binary storage size of sample
    unsigned int Event::getSize() const {
      return GetSize(waveform_->getArray().getFormat());
    }

    //! get binary storage size of sample
    unsigned int Event::GetSize(const Format::CP& dataFormat) {
      unsigned int gpsSize = GpsInfo::GetSize();

      unsigned int waveformSize = Waveform::GetSize(*dataFormat);

      //std::cout << "getSize() : gps: " << gpsSize << " wfm: " << waveformSize << " , # of samples: " << getNumberOfEvents() << std::endl;

      return gpsSize + waveformSize;
    }

    std::ostream& operator <<(std::ostream& os, const bo::data::Event &sample) {
      const data::Waveform& wfm = sample.getWaveform();
      const data::GpsInfo& gpsInfo = sample.getGpsInfo();

      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S.%f");
      std::locale oldLocale = os.imbue(std::locale(std::locale::classic(), timefacet));

      os.setf(std::ios::fixed);
      os.precision(4);
      unsigned int maxIndex = wfm.getMaxIndex();
      
      os << wfm.getTime(maxIndex) << " " << gpsInfo;
      os << " " << wfm.getTimeDelta().total_nanoseconds();

      os.precision(2);
      double squareSum = 0.0;
      for (unsigned int channel = 0; channel < wfm.getNumberOfChannels(); channel++) {
	double value = wfm.get(maxIndex, channel);
	os << " " << value;
	squareSum += value * value;
      }
      os << " " << sqrt(squareSum);
      os << " " << maxIndex;

      // restore original locale
      os.imbue(oldLocale);

      return os;
    }


  }
}
