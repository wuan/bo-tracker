#ifndef LIBBLITZORTUNG_UTIL_RINGBUFFER_H_
#define LIBBLITZORTUNG_UTIL_RINGBUFFER_H_

#include <boost/shared_array.hpp>

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

	public:

	  //! constructor  
	  RingBuffer(int size);

	  //! destructor
	  virtual ~RingBuffer();

	  //! add element to ring buffer
	  void add(const T &element);

	  //! return sum of buffer content
	  T getSum() const;

	  //! return average value of buffer content
	  T getAverage() const;

	  //! return actual size of data
	  int getActualSize() const;

      };

  }
}


#endif
