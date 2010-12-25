#ifndef BLITZORTUNG_DATA_FORMAT_H_
#define BLITZORTUNG_DATA_FORMAT_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    class Format {

      public:

	typedef std::auto_ptr<Format> AP;

      private:

	unsigned char numberOfBits_;

	unsigned char numberOfChannels_;

	unsigned short numberOfSamples_;

	unsigned char sampleByteSize_;

	unsigned int dataByteSize_;

	void updateSizes();

      public:

        //! create a waveform format object
	/*!
	\param bits number of bits per sample
	\param channels number of channels per measurement
	\param numberOfSamples number of measurements per waveform
	*/
	Format(unsigned char numberOfBits, unsigned char numberOfChannels, unsigned short numberOfSamples);

	//! create an invalid format
	Format();

	//! delete waveform object
	virtual ~Format();

	//! returns the number of bits
	unsigned char getNumberOfBitsPerSample() const;

	//! returns the number of channels per measurement
	unsigned char getNumberOfChannels() const;

	//! returns the number of measurements per waveform
	unsigned short getNumberOfSamples() const;

	//! returns the number of bytes per single sample
	unsigned short getBytesPerSample() const;

	//! returns the total size of the data according to the format
	unsigned int getDataSize() const;

	//! returns the index position of the data value
	unsigned int getIndex(unsigned short index, unsigned char channel) const;

	//! read the format parameters from stream
	void fromStream(std::iostream&);

	//! write format parameters to stream
	void toStream(std::iostream&) const;

	//! comparison operator for data format
	bool operator==(const Format& other) const;

	//! stream output operator
	std::ostream& operator<<(std::ostream& os) const;

    };


  }
}
#endif
