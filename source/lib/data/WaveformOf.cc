#include "data/Format.h"
#include "data/WaveformOf.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    template<typename T>
    WaveformOf<T>::WaveformOf(unsigned char numberOfChannels, unsigned short numberOfSamples, pt::ptime const& t0, pt::time_duration const& dt) :
      ArrayOf<T>(numberOfChannels, numberOfSamples),
      t0_(t0),
      dt_(dt)
    {
    }

    template<typename T>
    WaveformOf<T>::WaveformOf(unsigned char numberOfChannels, unsigned short numberOfSamples, const gr::date& date, std::iostream& stream) :
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
    WaveformOf<T>::~WaveformOf()
    {
    }

    template<typename T>
    const pt::ptime& WaveformOf<T>::getTime() const {
      return t0_;
    }

    template<typename T>
    pt::ptime WaveformOf<T>::getTime(unsigned int index) const {
      return t0_ + dt_ * index;
    }

    template<typename T>
    const pt::time_duration& WaveformOf<T>::getTimeDelta() const {
      return dt_;
    }

    template<typename T>
    unsigned int WaveformOf<T>::getStorageSize() const {
      util::Size size;

      long long int nanoseconds;
      size.add(nanoseconds);

      if (this->getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds;
	size.add(deltaNanoseconds);
      }

      return size.get() + ArrayOf<T>::getStorageSize();
    }

    template<typename T>
    void WaveformOf<T>::toStream(std::iostream& stream) const {
      unsigned long long int nanoseconds = t0_.time_of_day().total_nanoseconds();
      util::Stream::WriteValue(stream, nanoseconds);

      if (this->getNumberOfSamples() > 1) {
	unsigned short deltaNanoseconds = dt_.total_nanoseconds();
	util::Stream::WriteValue(stream, deltaNanoseconds);
      }

      ArrayOf<T>::toStream(stream);
    }

    template<typename T>
    unsigned int WaveformOf<T>::GetSize(const data::Format& dataFormat) {
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
    std::ostream& operator <<(std::ostream& os, const bo::data::WaveformOf<T>& wfm) {
      os << dynamic_cast<ArrayOf<T>>(wfm);

      return os;
    }

    //! explicit instatiation of functions to be linked afterwards
    template class WaveformOf<signed char>;
    template class WaveformOf<unsigned char>;
    template class WaveformOf<signed short>;
    template class WaveformOf<unsigned short>;
    template class WaveformOf<signed int>;
    template class WaveformOf<unsigned int>;
    template class WaveformOf<float>;
    template class WaveformOf<double>;

  }
}
