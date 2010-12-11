#include "data/sample/V1.h"
#include "util/Stream.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      V1::V1(Sample::Waveform::AP waveform, GpsInfo::AP gpsInfo) :
	Sample(waveform, gpsInfo)
      {
      }

      V1::V1(std::iostream& stream, const gr::date& date) :
        Sample(stream, date, getNumberOfSamples())
      {
      }

      unsigned short V1::getVersion() const {
	return 1;
      }

      unsigned short V1::getNumberOfSamples() const {
        return 1;
      }

      Sample::Waveform::AP V1::processWaveform() const {
	const Sample::Waveform& wfm = getWaveform();

	if (wfm.getNumberOfSamples() > getNumberOfSamples()) {
	  const int AD_MAX_VALUE = 128;
	  const int AD_MAX_VOLTAGE = 2500;
	  const int AD_THRESHOLD_VOLTAGE = 500;

	  float maxX = wfm.getMaxX();
	  float maxY = wfm.getMaxY();
	  int maxIndex = wfm.getMaxIndex();

	  // correction introduced with v 16 of the original tracker software
	  if ((abs(maxX) < AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE) &&
	      (abs(maxY) < AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE)) {

	    maxX = AD_MAX_VALUE*AD_THRESHOLD_VOLTAGE/AD_MAX_VOLTAGE;
	    maxY = 0.0;
	    maxIndex = -1;
	  }

	  pt::ptime timestamp = wfm.getTime() + wfm.getTimeDelta() * maxIndex;

	  Sample::Waveform::AP newwfm(new Sample::Waveform(timestamp));

	  newwfm->add(maxX, maxY);

	  return newwfm;
	}

	return Sample::Waveform::AP();
      }

    }
  }
}
