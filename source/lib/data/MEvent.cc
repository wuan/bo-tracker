#include "data/Format.h"
#include "data/MEvent.h"

namespace blitzortung {
  namespace data {

    MEvent::MEvent(Waveform::AP&& waveform, GpsInfo::AP&& gpsInfo, const std::string& rawData) :
      Event(std::move(waveform), std::move(gpsInfo)),
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
