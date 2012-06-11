#include "data/Array.h"

namespace blitzortung {
  namespace data {

    Array::~Array() = default;

    float Array::getAmplitude(unsigned int index) const {
      if (getNumberOfChannels() > 1) {
	return sqrt(getAmplitudeSquare(index));
      } else {
	return getFloat(index, 0);
      }
    }

    float Array::getAmplitudeSquare(unsigned int index) const {
      double sum = 0.0;

	for (int channel = 0; channel < getNumberOfChannels(); channel++) {
	  float amp = getFloat(index, channel);
	  sum += amp * amp;
	}

	return sum;
    }


    float Array::getPhase(unsigned int index) const {

      unsigned short channels = getNumberOfChannels();

      if (channels == 1) {
	return 0;
      } else {
	return atan2(getFloat(index, 1), getFloat(index, 0));
      }
    }

    inline float sqr(float x) {
      return x*x;
    }

    unsigned int Array::getMaxIndexInternal(bool ignoreClipped) const {
      unsigned int maxIndex = 0;
      float maxAmplitude = 0.0;

      float scaleFactorSquared = sqr(1 << (getElementSize() * 8 - 1));

      for (unsigned int sample=0; sample < getNumberOfSamples(); sample++) {
	double amplitude = getAmplitudeSquare(sample);
	if (amplitude > maxAmplitude) {
	  if (ignoreClipped) {
	    if (amplitude > scaleFactorSquared) {
	      continue;
	    }
	  }
	  maxIndex = sample;
	  maxAmplitude = amplitude;
	}
      }
      return maxIndex;
    }

    unsigned int Array::getMaxIndex() const {
      return getMaxIndexInternal(false);
    }

    unsigned int Array::getMaxIndexNoClip() const {
      return getMaxIndexInternal(true);
    }
  }
}
