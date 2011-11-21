#include "data/Waveform.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    Waveform::Waveform(const pt::ptime& t0) :
      t0_(t0)
    {
    }

    Waveform::Waveform(data::Array::AP&& array, const pt::ptime& t0, const pt::time_duration& dt) :
      t0_(t0),
      dt_(dt)
    {
      array_ = std::move(array);
    }

    Waveform::Waveform(const Format& dataFormat, gr::date date, std::iostream& stream) :
      array_(data::Array::AP(new data::Array(dataFormat)))
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
      if (dataFormat.getNumberOfSamples() > 1) {
	util::Stream::ReadValue(stream, deltaNanoseconds);
      }
      dt_ = pt::nanoseconds(deltaNanoseconds);

      array_->fromStream(stream);
    }

    Waveform::~Waveform()
    {
    }

    const pt::ptime& Waveform::getTime() const {
      return t0_;
    }

    pt::ptime Waveform::getTime(unsigned int index) const {
      return t0_ + dt_ * index;
    }

    const pt::time_duration& Waveform::getTimeDelta() const {
      return dt_;
    }

    float Waveform::get(unsigned int index, unsigned short channel) const {
      return array_->get(index, channel);
    }

    float Waveform::getAmplitude(unsigned int index) const {
      double sum = 0.0;
      unsigned short channels = array_->getNumberOfChannels();

      if (channels > 1) {
	for (int channel = 0; channel < channels; channel++) {
	  float amp = get(index, channel);
	  sum += amp * amp;
	}

	return sqrt(sum);
      } else {
	return get(index, 0);
      }
    }

    unsigned int Waveform::getMaxIndex() const {
      unsigned int maxIndex = 0;
      float maxAmplitude = 0.0;
      for (unsigned int sample=0; sample < array_->getNumberOfSamples(); sample++) {
	double amplitude = getAmplitude(sample);
	if (amplitude > maxAmplitude) {
	  maxIndex = sample;
	  maxAmplitude = amplitude;
	}
      }
      return maxIndex;
    }

    unsigned int Waveform::getNumberOfSamples() const {
      return array_->getNumberOfSamples();
    }

    unsigned int Waveform::getNumberOfChannels() const {
      return array_->getNumberOfChannels();
    }

    void Waveform::toStream(std::iostream& stream) {

      unsigned long long int nanoseconds = t0_.time_of_day().total_nanoseconds();
      util::Stream::WriteValue(stream, nanoseconds);

      if (array_->getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds = dt_.total_nanoseconds();
	util::Stream::WriteValue(stream, deltaNanoseconds);
      }

      array_->toStream(stream);
    }

    void Waveform::updateFormatRef(const Format& format) {
      if (format != array_->getFormat())
	std::cout << std::endl << "update format ref format mismatch " << format << " vs. " << array_->getFormat() << std::endl;
      assert(format == array_->getFormat());
      array_ = Array::AP(new Array(*array_, format));
    }

    bool Waveform::isEmpty() const {
      return array_.get() == 0;
    }
	
    const Array& Waveform::getArray() const {
      return *array_;
    }
	
    unsigned int Waveform::GetSize(const data::Format& dataFormat) {
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

    std::ostream& operator <<(std::ostream& os, const bo::data::Waveform& wfm) {
      os << wfm.getArray();

      return os;
    }

  }
}
