/*
    blitzortung-lib shared components for Blitzortung.org related software
    Copyright (C) 2010-12 Andreas Würl <blitzortung@tryb.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
