
#include "exception/Base.h"
#include "util/RingBuffer.h"

namespace blitzortung {
  namespace util {

    // TODO calcluating the sum by summing up all elemnts with a given type can cause problems with overflows occuring, this has to be circumvented in a future release

    template <typename T> RingBuffer<T>::RingBuffer(int size) {
      size_ = size;

      // allocate data array
      data_ = 0;
      data_ = new T[size_];

      // initialize parameters
      pos_ = 0;
      actualSize_ = 0;
    }

    template <typename T> RingBuffer<T>::~RingBuffer() {
      if (data_ != 0) {
	delete data_;
        data_ = 0;
      }
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

    template <typename T> T RingBuffer<T>::getSum() const {
      double sum = 0.0;

      for (int i=0; i < actualSize_; i++) {
	sum += data_[i];
      }
      return sum;
    }

    template <typename T> T RingBuffer<T>::getAverage() const {

      if (actualSize_ == 0)
	throw exception::Base("util::RingBuffer::getAverage() no average value of empty dataset"); 

      return getSum() / actualSize_;
    }

    template <typename T> int RingBuffer<T>::getActualSize() const {
      return actualSize_;
    }

    //! explicit instatiation of functions to be linked afterwards
    template class RingBuffer<double>;
    template class RingBuffer<float>;
    template class RingBuffer<long int>;
    template class RingBuffer<long unsigned int>;
    template class RingBuffer<int>;
    template class RingBuffer<unsigned int>;
    template class RingBuffer<char>;
    template class RingBuffer<unsigned char>;

  }
}
