#include "data/WaveformOf.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    template<typename T>
    Waveform<T>::Waveform(unsigned char numberOfChannels, unsigned short numberOfSamples, const pt::ptime& t0, const pt::time_duration& dt) :
      ArrayOf<T>(numberOfChannels, numberOfSamples),
      t0_(t0),
      dt_(dt)
    {
    }

    template<typename T>
    Waveform<T>::Waveform(unsigned char numberOfChannels, unsigned short numberOfSamples, const gr::date& date, std::iostream& stream) :
      ArrayOf<T>(numberOfChannels, numberOfSamples)
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

      ArrayOf<T>::fromStream(stream);
    }

    template<typename T>
    Waveform<T>::~Waveform()
    {
    }

    template<typename T>
    const pt::ptime& Waveform<T>::getTime() const {
      return t0_;
    }

    template<typename T>
    pt::ptime Waveform<T>::getTime(unsigned int index) const {
      return t0_ + dt_ * index;
    }

    template<typename T>
    const pt::time_duration& Waveform<T>::getTimeDelta() const {
      return dt_;
    }

    template<typename T>
    void Waveform<T>::toStream(std::iostream& stream) {
      unsigned long long int nanoseconds = t0_.time_of_day().total_nanoseconds();
      util::Stream::WriteValue(stream, nanoseconds);

      if (this->getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds = dt_.total_nanoseconds();
	util::Stream::WriteValue(stream, deltaNanoseconds);
      }

      Array::toStream(stream);
    }

    template<typename T>
    unsigned int Waveform<T>::GetSize(const data::Format& dataFormat) {
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

    template<typename T>
    std::ostream& operator <<(std::ostream& os, const bo::data::Waveform<T>& wfm) {
      os << dynamic_cast<ArrayOf<T>>(wfm);

      return os;
    }

  }
}
