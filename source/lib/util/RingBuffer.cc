#include <cmath>

#include "exception/Base.h"
#include "util/RingBuffer.h"

namespace blitzortung {
  namespace util {

    // TODO calcluating the sum by summing up all elemnts with a given type can cause problems with overflows occuring, this has to be circumvented in a future release

    template <typename T> RingBuffer<T>::RingBuffer(int size) :
      data_(new T[size]),
      pos_(0),
      size_(size),
      actualSize_(0),
      logger_("RingBuffer")
    {
    }

    template <typename T> RingBuffer<T>::~RingBuffer() {
    }

    template <typename T> void RingBuffer<T>::add(const T &element) {
      // add element to ring buffer

      // write element to the buffer
      data_[pos_] = element;

      // move write position
      pos_ = ( pos_ + 1 ) % size_;

      // adapt actual buffer size
      if (actualSize_ < size_) 
	actualSize_++;
    }

    template <typename T> double RingBuffer<T>::getSum() const {
      double sum = 0.0;

      for (int i=0; i < actualSize_; i++) {
	sum += data_[i];
      }
      return sum;
    }

    template <typename T> double RingBuffer<T>::getAverage() const {

      if (actualSize_ == 0)
	throw exception::Base("util::RingBuffer::getAverage() no average value of empty dataset"); 

      return getSum() / actualSize_;
    }

    template <typename T> double RingBuffer<T>::getStddev() const {

      if (actualSize_ == 0)
	throw exception::Base("util::RingBuffer::getStddev() standard deviation of empty dataset cannot be calculated"); 

      double average = getAverage();
      double sum = 0.0;

      for (int i=0; i < actualSize_; i++) {
	sum += sqr(data_[i] - average);
      }

      //logger_.infoStream() << "getStddev() avg " << average << ", sum " << sum << ", actSize " << actualSize_ << ", ѕtddev " << sqrt(sum) / actualSize_;
      return sqrt(sum) / actualSize_;
    }

    template <typename T> int RingBuffer<T>::getActualSize() const {
      return actualSize_;
    }

    template <typename T> int RingBuffer<T>::getSize() const {
      return size_;
    }

    template <typename T> T RingBuffer<T>::sqr(T x) const {
      return x*x;
    }

    template <typename T> T RingBuffer<T>::operator[](unsigned int index) const {
      // move write position
      logger_.debugStream() << "pos: " << pos_ << ", size " << size_ << ", index " << index << ", idx = " << ( pos_ + index ) % size_;
      int start = (pos_ - actualSize_) % size_;
      return data_[( start + index ) % size_];
    }

    //! explicit instatiation of functions to be linked afterwards
    template class RingBuffer<double>;
    template class RingBuffer<float>;
    template class RingBuffer<long long int>;
    template class RingBuffer<unsigned long long int>;
    template class RingBuffer<int>;
    template class RingBuffer<unsigned int>;
    template class RingBuffer<short>;
    template class RingBuffer<unsigned short>;
    template class RingBuffer<char>;
    template class RingBuffer<unsigned char>;

  }
}
