#include "data/Format.h"
#include "data/WaveformOf.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    template<typename T, int numberOfSamples, int numberOfChannels>
    WaveformOf<T, numberOfSamples, numberOfChannels>::WaveformOf(pt::ptime const& t0, pt::time_duration const& dt) :
      t0_(t0),
      dt_(dt)
    {
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    WaveformOf<T, numberOfSamples, numberOfChannels>::WaveformOf(const gr::date& date, std::iostream& stream) :
      t0_(),
      dt_()
    {
      {
	unsigned long long int nanoseconds;
	util::Stream::ReadValue(stream, nanoseconds);

	// fixed nanosecond to time conversion
	int seconds = nanoseconds / 1000000000ULL;
	nanoseconds %= 1000000000ULL;

	t0_ = pt::ptime(date, pt::seconds(seconds) + pt::nanoseconds(nanoseconds));
      }

      unsigned short deltaNanoseconds = 0;
      if (numberOfSamples > 1) {
	util::Stream::ReadValue(stream, deltaNanoseconds);
      }
      dt_ = pt::nanoseconds(deltaNanoseconds);

      ArrayOf<T, numberOfSamples, numberOfChannels>::fromStream(stream);
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    WaveformOf<T, numberOfSamples, numberOfChannels>::~WaveformOf()
    {
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    const pt::ptime& WaveformOf<T, numberOfSamples, numberOfChannels>::getTimestamp() const {
      return t0_;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    pt::ptime WaveformOf<T, numberOfSamples, numberOfChannels>::getTimestamp(unsigned int index) const {
      return t0_ + dt_ * index;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    const pt::time_duration& WaveformOf<T, numberOfSamples, numberOfChannels>::getTimeDelta() const {
      return dt_;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    unsigned int WaveformOf<T, numberOfSamples, numberOfChannels>::getStorageSize() const {
      util::Size size;

      long long int nanoseconds;
      size.add(nanoseconds);

      if (this->getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds;
	size.add(deltaNanoseconds);
      }

      return size.get() + ArrayOf<T, numberOfSamples, numberOfChannels>::getStorageSize();
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    void WaveformOf<T, numberOfSamples, numberOfChannels>::toStream(std::iostream& stream) const {
      unsigned long long int nanoseconds = t0_.time_of_day().total_nanoseconds();
      util::Stream::WriteValue(stream, nanoseconds);

      if (this->getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds = dt_.total_nanoseconds();
	util::Stream::WriteValue(stream, deltaNanoseconds);
      }

      ArrayOf<T, numberOfSamples, numberOfChannels>::toStream(stream);
    }

    inline float sqr(float x) {
      return x*x;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    json_object* WaveformOf<T, numberOfSamples, numberOfChannels>::asJson(bool normalize) const {
      json_object* jsonArray = json_object_new_array();

      if (normalize && numberOfChannels == 2) {
	//numberOfChannels = 1;
      } else {
	normalize = false;
      }

      json_object** values = new json_object*[numberOfChannels];

      for (unsigned short channel = 0; channel < numberOfChannels; channel++) {
	values[channel] = json_object_new_array();
	json_object_array_add(jsonArray, values[channel]);
      }

      float scaleFactor = 1 << (getElementSize() * 8 - 1);

      float angle = -getPhase(getMaxIndexNoClip());
      if (normalize) {
	float angle_cos = cos(angle);
	float angle_sin = sin(angle);
	float deviation = 0.0;
	for (unsigned int sample = 0; sample < getNumberOfSamples(); sample++) {
	    float x_value = getFloat(sample, 0) / scaleFactor;
	    float y_value = getFloat(sample, 1) / scaleFactor;
	    json_object_array_add(values[0], json_object_new_double(x_value * angle_cos - y_value * angle_sin));
	    deviation = sqr(x_value * angle_sin) + sqr(y_value * angle_cos);
	}
	json_object_array_add(jsonArray, json_object_new_double(sqrt(deviation)));
      } else {
	for (unsigned int sample = 0; sample < getNumberOfSamples(); sample++) {
	  for (unsigned short channel = 0; channel < numberOfChannels; channel++) {
	    json_object_array_add(values[channel], json_object_new_double(getFloat(sample, channel) / scaleFactor));
	  }
	}
      }

      delete values;

      return jsonArray;
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    unsigned int WaveformOf<T, numberOfSamples, numberOfChannels>::GetSize(const data::Format& dataFormat) {
      util::Size size;

      long long int nanoseconds;
      size.add(nanoseconds);

      if (dataFormat.getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds;
	size.add(deltaNanoseconds);
      }

      // gps data size + two times the size of an element (for x and y value)
      return size.get() + dataFormat.getDataSize();
    }

    template<typename T, int numberOfSamples, int numberOfChannels>
    std::ostream& operator <<(std::ostream& os, const bo::data::WaveformOf<T, numberOfSamples, numberOfChannels>& wfm) {
      os << dynamic_cast<ArrayOf<T, numberOfSamples, numberOfChannels>>(wfm);

      return os;
    }

    //! explicit instatiation of functions to be linked afterwards
    template class WaveformOf<signed char, 0  , 1>;
    template class WaveformOf<signed char, 1  , 1>;
    template class WaveformOf<signed char, 64 , 1>;
    template class WaveformOf<signed char, 128, 1>;
    template class WaveformOf<signed char, 256, 1>;
    template class WaveformOf<signed char, 1  , 2>;
    template class WaveformOf<signed char, 64 , 2>;
    template class WaveformOf<signed char, 128, 2>;
    template class WaveformOf<signed char, 256, 2>;
    template class WaveformOf<signed short, 1, 2>;

  }
}
