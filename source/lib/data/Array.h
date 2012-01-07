#ifndef BLITZORTUNG_DATA_ARRAY_H_
#define BLITZORTUNG_DATA_ARRAY_H_

#include "namespaces.h"
#include "data/Format.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveform arrays
    class Array : boost::noncopyable {

      public:

	typedef std::unique_ptr<Array> AP;

      private:

	//! reference to data format object
	const Format& format_;

	//! vector for x channel data
	std::unique_ptr<char> data_;

      public:

        //! create a waveform array object
	/*!
	\param format reference to array format definition
	*/
	Array(const Format& format);

	Array(Array&& old, const Format& format);

	//! delete waveform object
	virtual ~Array();

	//! returns the number of bits per sample
	unsigned short getNumberOfBitsPerSample() const;

	//! returns the number of measurements per waveform
	unsigned int getNumberOfSamples() const;

	//! returns the number of channels per measurement
	unsigned short getNumberOfChannels() const;

	//! set a particular sample in a given channel to a value
	void set(int value, unsigned int sample, unsigned short channel=0);

	//! return the value of a sample in a given channel
	float get(unsigned int sample, unsigned short channel=0) const;

	//! write binary object data to stream
	void toStream(std::iostream&) const;

	//! read binary object data from stream
	void fromStream(std::iostream&);

	//! return the actual format
	const Format& getFormat() const;
	
    };

    std::ostream& operator <<(std::ostream& os, const Array&);

  }
}
#endif
