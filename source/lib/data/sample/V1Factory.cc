#include "data/sample/V1Factory.h"
#include "data/sample/V1.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      Sample::AP V1Factory::createSample(Sample::Waveform::AP wfm, GpsInfo::AP gpsInfo) const {
	return Sample::AP(new V1(wfm, gpsInfo));
      }

      Sample::AP V1Factory::createSample(std::iostream& stream, const gr::date& date) const {
	return Sample::AP(new V1(stream, date));
      }

    }
  }
}
