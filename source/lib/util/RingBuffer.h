#ifndef LIBBLITZORTUNG_UTIL_RINGBUFFER_H_
#define LIBBLITZORTUNG_UTIL_RINGBUFFER_H_

#include <boost/shared_array.hpp>

#include "Logger.h"

namespace blitzortung {
  namespace util {

    template <typename T>
      class RingBuffer {

	private:
	  //! pointer to data array
	  boost::shared_array<T> data_;

	  //! actual data write position
	  int pos_;

	  //! total size of data array
	  int size_;

	  //! actual size of data array
	  int actualSize_;

	  //! logger for this class
	  mutable Logger logger_;

	  T sqr(T x) const;

	public:

	  //! constructor  
	  RingBuffer(int size);

	  //! destructor
	  virtual ~RingBuffer();

	  //! add element to ring buffer
	  void add(const T &element);

	  //! return sum of buffer content
	  double getSum() const;

	  //! return average value of buffer content
	  double getAverage() const;
	  
	  //! return standard deviation of buffer content
	  double getStddev() const;

	  //! return actual size of data in buffer
	  int getActualSize() const;

	  //! return size of buffer
	  int getSize() const;

	  //! index operation
	  T operator[](unsigned int index) const;
      };

  }
}


#endif
