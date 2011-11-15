#ifndef BLITZORTUNG_DATA_FORMAT_H_
#define BLITZORTUNG_DATA_FORMAT_H_

#include "namespaces.h"
#include "Logger.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    class Format {

      public:

        // typedef for Format shared pointer
	typedef std::shared_ptr<const Format> CP;

	// enum for the storage type of single data values
	enum Type {BYTE=1, SHORT=2, INT=4};

      private:

	//! number of bits per sample
	unsigned char numberOfBits_;

	//! number of channels
	unsigned char numberOfChannels_;

	//! number of samples per channel
	unsigned short numberOfSamples_;

	//! intermediate value of bytes per sample value
	Type sampleType_;

	//! logger for this class
	mutable Logger logger_;

	//! update the data storage type according to the number of bits per sample value
	void updateDataType();

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

	//! construct from stream
	Format(std::iostream&);

	//! delete waveform object
	virtual ~Format();

	//! returns the number of bits per sample
	unsigned short getNumberOfBitsPerSample() const;

	//! returns the number of bits per sample
	int getSampleZeroOffset() const;

	//! returns the number of channels per measurement
	unsigned short getNumberOfChannels() const;

	//! returns the number of measurements per waveform
	unsigned short getNumberOfSamples() const;

	//! returns the number of bytes per single sample
	unsigned short getNumberOfBytesPerSample() const;

	//! get data type of format
	Type getDataType() const;

	//! returns the total size of the data according to the format
	unsigned int getDataSize() const;

	//! returns the index position of the data value
	unsigned int getIndex(unsigned short index, unsigned char channel) const;

	//! write format parameters to stream
	void toStream(std::iostream&) const;

	//! comparison operator for data format
	bool operator==(const Format& other) const;

	//! comparison operator for data format
	bool operator!=(const Format& other) const;

    };

    //! stream output operator
    std::ostream& operator<<(std::ostream& os, const Format& format);

  }
}
#endif
