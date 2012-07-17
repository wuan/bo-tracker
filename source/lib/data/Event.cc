#include "data/Format.h"
#include "data/Event.h"

namespace blitzortung {
  namespace data {

    Event::Event(Waveform::AP&& waveform, GpsInfo::AP&& gpsInfo) :
      waveform_(std::move(waveform)),
      gpsInfo_(std::move(gpsInfo))
    {
    }

    Event::Event(const data::Format& format, const gr::date& date, std::iostream& stream) :
      waveform_(format.createWaveformFromStream(date, stream)),
      gpsInfo_(GpsInfo::AP(new GpsInfo(stream)))
    {
    }

    Event::~Event() = default;

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
      return waveform_->getTimestamp() < rhs.waveform_->getTimestamp();
    } 

    bool Event::operator==(const Event &rhs) const {
      return waveform_->getTimestamp() == rhs.waveform_->getTimestamp();
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

    //! get binary storage size of event
    size_t Event::getStorageSize() const {
      size_t gpsSize = GpsInfo::GetSize();

      size_t waveformSize = waveform_->getStorageSize();

      return gpsSize + waveformSize;
    }

    //! get binary storage size of event
    size_t Event::GetSize(const Format& dataFormat) {
      size_t gpsSize = GpsInfo::GetSize();

      size_t waveformSize = Waveform::GetSize(dataFormat);

      return gpsSize + waveformSize;
    }

    const pt::ptime& Event::getTimestamp() const {
      return waveform_->getTimestamp();
    }

    pt::ptime Event::getTimestamp(unsigned int index) const {
      return waveform_->getTimestamp(index);
    }

    json_object* Event::asJson() const {
      json_object* jsonArray = json_object_new_array();

      pt::time_facet *timefacet = new pt::time_facet();
      timefacet->format("%Y-%m-%d %H:%M:%S");

      std::ostringstream oss;
      std::locale oldLocale = oss.imbue(std::locale(std::locale::classic(), timefacet));

      oss << waveform_->getTimestamp();

      json_object_array_add(jsonArray, json_object_new_string(oss.str().c_str()));

      json_object_array_add(jsonArray, json_object_new_int(waveform_->getTimestamp().time_of_day().fractional_seconds()));
      json_object_array_add(jsonArray, json_object_new_double(gpsInfo_->getLongitude()));
      json_object_array_add(jsonArray, json_object_new_double(gpsInfo_->getLatitude()));
      json_object_array_add(jsonArray, json_object_new_int(gpsInfo_->getAltitude()));
      json_object_array_add(jsonArray, json_object_new_int(gpsInfo_->getNumberOfSatellites())); 
      json_object_array_add(jsonArray, json_object_new_int(waveform_->getTimeDelta().total_nanoseconds()));

      float scaleFactor = 1 << (waveform_->getElementSize() * 8 - 1);
      json_object_array_add(jsonArray, json_object_new_double(waveform_->getAmplitude(waveform_->getMaxIndex())/scaleFactor));
      json_object_array_add(jsonArray, json_object_new_double(waveform_->getPhase(waveform_->getMaxIndexNoClip())));
      json_object_array_add(jsonArray, json_object_new_int(waveform_->getMaxIndex()));

      return jsonArray;
    }

    std::ostream& operator <<(std::ostream& os, const bo::data::Event &event) {
      const data::Waveform& wfm = event.getWaveform();
      const data::GpsInfo& gpsInfo = event.getGpsInfo();

      os.setf(std::ios::fixed);
      os.precision(4);
      unsigned int maxIndex = wfm.getMaxIndex();
      
      os << event.getTimestamp(maxIndex) << " " << gpsInfo;
      os << " " << wfm.getTimeDelta().total_nanoseconds();

      os.precision(2);
      float scaleFactor = 1 << (wfm.getElementSize() * 8 - 1);
      os << " " << wfm.getAmplitude(wfm.getMaxIndex())/scaleFactor;
      os << " " << wfm.getPhase(wfm.getMaxIndexNoClip());
      os << " " << wfm.getMaxIndex();

      return os;
    }


  }
}
