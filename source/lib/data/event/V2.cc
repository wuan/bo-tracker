#include "data/sample/V2.h"
#include "util/Stream.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      V2::V2(Sample::Waveform::AP waveform, GpsInfo::AP gpsInfo) :
	Sample(waveform, gpsInfo)
      {
      }

      V2::V2(std::iostream& stream, const gr::date& date) :
        Sample(stream, date, getNumberOfSamples())
      {
      }

      unsigned short V2::getVersion() const {
	return 2;
      }

      unsigned short V2::getNumberOfSamples() const {
        return 128;
      }

      Sample::Waveform::AP V2::processWaveform() const {
	return Sample::Waveform::AP();
      }

    }
  }
}
