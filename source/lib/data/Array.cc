#include "data/Array.h"

namespace blitzortung {
  namespace data {

    Array::~Array() = default;

    float Array::getAmplitude(unsigned int index) const {
      double sum = 0.0;
      unsigned short channels = getNumberOfChannels();

      if (channels > 1) {
	for (int channel = 0; channel < channels; channel++) {
	  float amp = getFloat(index, channel);
	  sum += amp * amp;
	}

	return sqrt(sum);
      } else {
	return getFloat(index, 0);
      }
    }

    float Array::getPhase(unsigned int index) const {

      unsigned short channels = getNumberOfChannels();

      if (channels == 1) {
	return 0;
      } else {
	return atan2(getFloat(index, 1), getFloat(index, 0));
      }
    }


    unsigned int Array::getMaxIndex() const {
      unsigned int maxIndex = 0;
      float maxAmplitude = 0.0;
      for (unsigned int sample=0; sample < getNumberOfSamples(); sample++) {
	double amplitude = getAmplitude(sample);
	if (amplitude > maxAmplitude) {
	  maxIndex = sample;
	  maxAmplitude = amplitude;
	}
      }
      return maxIndex;
    }

  }
}
