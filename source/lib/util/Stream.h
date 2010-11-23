#ifndef BLITZORTUNG_UTIL_STREAM_H_
#define BLITZORTUNG_UTIL_STREAM_H_

#include <iostream>
#include <string>

namespace blitzortung {
  namespace util {

    class Stream {
      public:

	  //! template function for writing of different types to stream
	  template< typename T>
	    static void WriteValue(std::iostream& stream, const T& value) {
	      stream.write((char*) &value, sizeof(value));
	    }
	    
	  template< typename T>
	    static void ReadValue(std::iostream& stream, T& value) {
	      stream.read((char*) &value, sizeof(value));
	    }
    };

  }
}

#endif
