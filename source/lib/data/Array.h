#ifndef BLITZORTUNG_DATA_ARRAY_H_
#define BLITZORTUNG_DATA_ARRAY_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/Format.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveform arrays
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

	unsigned int getNumberOfSamples() const;

	unsigned short getNumberOfChannels() const;

	void set(int value, unsigned int sample, unsigned short channel=0);

	int get(unsigned int sample, unsigned short channel=0) const;

	//! write binary object data to stream
	void toStream(std::iostream&) const;

	//! read binary object data from stream
	void fromStream(std::iostream&);

	const data::Format& getFormat() const;

    };

    std::ostream& operator <<(std::ostream& os, const Array&);

  }
}
#endif
