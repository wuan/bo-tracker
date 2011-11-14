#include "data/MEvent.h"

namespace blitzortung {
  namespace data {

    MEvent::MEvent(Waveform::AP& waveform, GpsInfo::AP& gpsInfo, const std::string& rawData) :
      Event(waveform, gpsInfo),
      rawData_(rawData)
    {
    }

    MEvent::~MEvent() {
    }

    const std::string& MEvent::getRawData() const {
      return rawData_;
    }

  }
}
