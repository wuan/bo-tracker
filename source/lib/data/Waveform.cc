#include "data/Waveform.h"
#include "util/Stream.h"
#include "util/Size.h"

namespace blitzortung {
  namespace data {

    template <typename T>
    Waveform<T>::Waveform(const pt::ptime& t0, const pt::time_duration& dt) :
      t0_(t0),
      dt_(dt)
    {
    }

    template <typename T>
    Waveform<T>::Waveform(std::iostream& stream, gr::date date, const unsigned int elements) {
      {
	unsigned long long int nanoseconds;
	util::Stream::ReadValue(stream, nanoseconds);

	// fixed nanosecond to time conversion
	int seconds = nanoseconds / 1000000000ULL;
	nanoseconds %= 1000000000ULL;

	t0_ = pt::ptime(date, pt::seconds(seconds) + pt::nanoseconds(nanoseconds));
      }

      unsigned short deltaNanoseconds = 0;
      if (elements > 1) {
	util::Stream::ReadValue(stream, deltaNanoseconds);
      }
      dt_ = pt::nanoseconds(deltaNanoseconds);

      for (unsigned int i=0; i < elements; i++) {
	T xvalue, yvalue;
	util::Stream::ReadValue(stream, xvalue);
	util::Stream::ReadValue(stream, yvalue);
	add(xvalue, yvalue);
      }

    }

    template <typename T>
    Waveform<T>::~Waveform()
    {
    }

    template <typename T>
    void Waveform<T>::add(T xval, T yval) {
      float val = xval * xval + yval * yval;

      if (xdata_.size() == 0 || val > maxVal_) {
	maxVal_ = val;
	maxIndex_ = xdata_.size();
      }

      xdata_.push_back(xval);
      ydata_.push_back(yval);
    }

    template <typename T>
      const pt::ptime& Waveform<T>::getTime() const {
	return t0_;
      }
      
    template <typename T>
      pt::ptime Waveform<T>::getTime(unsigned int index) const {
	return t0_ + dt_ * index;
      }

    template <typename T>
      const pt::time_duration& Waveform<T>::getTimeDelta() const {
	return dt_;
      }
      
    template <typename T>
    T Waveform<T>::getX(unsigned int index) const {
      if (index < xdata_.size()) {
	return xdata_[index];
      } else {
	throw exception::Base("Waveform::getX() index out of range");
      }
    }

    template <typename T>
    T Waveform<T>::getY(unsigned int index) const {
      if (index < ydata_.size()) {
	return ydata_[index];
      } else {
	throw exception::Base("Waveform::getY() index out of range");
      }
    }

    template <typename T>
    float Waveform<T>::getAmplitude(unsigned int index) const {
      float xamp = getX(index);
      float yamp = getY(index);

      return sqrt(xamp * xamp + yamp * yamp);
    }

    template <typename T>
    unsigned int Waveform<T>::getMaxIndex() const {
      if (xdata_.size() ==0)
	throw exception::Base("data::Waveform::getMaxIndex() data arrays empty");

      return maxIndex_;
    }

    template <typename T>
    T Waveform<T>::getMaxX() const {
      if (xdata_.size() ==0)
	throw exception::Base("data::Waveform::getMaxX() data arrays empty");

      return xdata_[maxIndex_];
    }

    template <typename T>
    T Waveform<T>::getMaxY() const {
      if (ydata_.size() ==0)
	throw exception::Base("data::Waveform::getMaxY() data arrays empty");

      return ydata_[maxIndex_];
    }

    template <typename T>
    void Waveform<T>::write(std::iostream& stream, unsigned int elementCount) {

      unsigned long long int nanoseconds = t0_.time_of_day().total_nanoseconds();
      util::Stream::WriteValue(stream, nanoseconds);

      unsigned int elements = xdata_.size();
      if (elements != elementCount)
	throw exception::Base("data::Waveform::write() element count mismatch");

      if (elements > 1) {
	unsigned short deltaNanoseconds = dt_.total_nanoseconds();
	util::Stream::WriteValue(stream, deltaNanoseconds);
      }

      for (unsigned int i=0; i < elements; i++) {
	util::Stream::WriteValue(stream, xdata_[i]);
	util::Stream::WriteValue(stream, ydata_[i]);
      }
    }

    template <typename T>
      unsigned int Waveform<T>::GetSize(unsigned int elements) {
	util::Size size;

	long long int nanoseconds;
	size.add(nanoseconds);

	if (elements > 1) {
	  unsigned short deltaNanoseconds;
	  size.add(deltaNanoseconds);
	}

	util::Size elementSize;

	T element;
	elementSize.add(element);

	// gps data size + two times the size of an element (for x and y value)
	return size.get() + elementSize.get() * 2 * elements;
      }

    //! explicit instatiation of functions to be linked afterwards
    template class Waveform<double>;
    template class Waveform<float>;
    template class Waveform<long int>;
    template class Waveform<long unsigned int>;
    template class Waveform<int>;
    template class Waveform<unsigned int>;
    template class Waveform<short>;
    template class Waveform<unsigned short>;
    template class Waveform<char>;
    template class Waveform<unsigned char>;
	
  }
}
