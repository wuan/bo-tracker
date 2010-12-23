#ifndef BLITZORTUNG_DATA_ARRAY_H_
#define BLITZORTUNG_DATA_ARRAY_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/Format.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    class Array : boost::noncopyable {

      public:

	typedef std::auto_ptr<Array> AP;

      private:

	//! reference to data format object
	const Format& format_;

	//! vector for x channel data
	unsigned char* data_;

      public:

        //! create a waveform array object
	/*!
	\param format reference to array format definition
	*/
	Array(const Format& format);

	//! delete waveform object
	virtual ~Array();

	unsigned int getIndex(unsigned int sample, unsigned short channel=0);
    };

    std::ostream& operator <<(std::ostream& os, const bo::data::Array& array) {
      return os;
    }

  }
}
#endif
