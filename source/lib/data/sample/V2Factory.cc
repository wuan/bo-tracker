#include "data/sample/V2Factory.h"
#include "data/sample/V2.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      Sample::AP V2Factory::createSample(Sample::Waveform::AP wfm, GpsInfo::AP gpsInfo) const {
	return Sample::AP(new V2(wfm, gpsInfo));
      }

      Sample::AP V2Factory::createSample(std::iostream& stream, const gr::date& date) const {

	return Sample::AP(new V2(stream, date));
      }


    }
  }
}
