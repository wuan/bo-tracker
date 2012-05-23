#ifndef BLITZORTUNG_UTIL_SIZE_H_
#define BLITZORTUNG_UTIL_SIZE_H_


namespace blitzortung {
  namespace util {

    //! class Size to determine sizes of binary structures
    class Size {

      private:

	//! internal variable for gathering sizes
	size_t size_;

      public:

	Size() :
	  size_(0)
      {
      }

	template<typename T>
	  size_t add(const T& value) {
	    size_ += sizeof(value);
	    return size_;
	  }

	size_t get() const {
	  return size_;
	}
    };

  }
}

#endif
