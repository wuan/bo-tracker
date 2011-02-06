#include "data/Event.h"

namespace blitzortung {
  namespace data {

    Event::Event(Waveform::AP waveform, GpsInfo::AP gpsInfo) :
      waveform_(waveform),
      gpsInfo_(gpsInfo)
    {
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
      return waveform_;
    }

    const GpsInfo& Event::getGpsInfo() const {
      return *gpsInfo_;
    }

    GpsInfo::AP Event::releaseGpsInfo() {
      return gpsInfo_;
    }

    bool Event::operator<(const Event &rhs) const {
      return waveform_->getTime() < rhs.waveform_->getTime();
    } 

    bool Event::CompareAmplitude::operator()(const first_argument_type &x, const second_argument_type &y) const {
      return x.getWaveform().getAmplitude(0) < y.getWaveform().getAmplitude(0);
    } 

    unsigned short Event::getNumberOfSamples() const {
      return waveform_->getNumberOfSamples();
    }

    void Event::toStream(std::iostream& stream) const {

      // write waveform to stream
      waveform_->toStream(stream);

      // write gps information to stream
      gpsInfo_->toStream(stream);
    }

    void Event::fromStream(data::Format::P& dataFormat, const gr::date& date, std::iostream& stream) {
      // read waveform from stream
      waveform_ = Waveform::AP(new Waveform(dataFormat, date, stream));

      // read gps information from stream
      gpsInfo_ = GpsInfo::AP(new GpsInfo(stream));

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
      os << " " << wfm.get(maxIndex, 0);
      os << " " << wfm.get(maxIndex, 1);
      os << " " << maxIndex;

      // restore original locale
      os.imbue(oldLocale);

      return os;
    }


  }
}
