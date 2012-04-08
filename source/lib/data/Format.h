#ifndef BLITZORTUNG_DATA_FORMAT_H_
#define BLITZORTUNG_DATA_FORMAT_H_

#include "namespaces.h"
#include "Logger.h"
#include "Waveform.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    class Format {

      public:

	// enum for the storage type of single data values
	enum class Type : unsigned short {BYTE=sizeof(char), SHORT=sizeof(short), INT=sizeof(int)};

      private:

	//! storage type of samples
	Type sampleType_;

	//! number of channels
	unsigned char numberOfChannels_;

	//! number of samples per channel
	unsigned short numberOfSamples_;

	//! logger for this class
	mutable Logger logger_;

	//! update the data storage type according to the number of bits per sample value
	void updateSampleType(unsigned char numberOfBytes);

      public:

        //! create a waveform format object
	/*!
	\param numberOfBits number of bits per sample
	\param numberOfChannels number of channels per measurement
	\param numberOfSamples number of measurements per waveform
	*/
	Format(unsigned char sampleStorageSize, unsigned char numberOfChannels, unsigned short numberOfSamples);

	//! create an invalid format
	Format();

	//! construct from stream
	Format(std::iostream&);

	//! delete waveform object
	virtual ~Format();

	Format& operator=(const Format& other);

	//! returns the number of channels per measurement
	unsigned char getNumberOfChannels() const;

	//! returns the number of measurements per waveform
	unsigned short getNumberOfSamples() const;

	//! returns the number of bytes per single sample
	unsigned char getNumberOfBytesPerSample() const;

	//! get data type of format
	Type getDataType() const;

	//! returns the total size of the data according to the format
	unsigned int getDataSize() const;

    	//! returns true if format is valid
	bool isValid() const;

	//! write format parameters to stream
	void toStream(std::iostream&) const;

	//! read format parameters from stream
	void fromStream(std::iostream&);

	//! create waveform from stream
	Waveform::AP createWaveform(const pt::ptime&, const pt::time_duration&) const;

	//! create waveform from stream
	Waveform::AP createWaveformFromStream(const gr::date&, std::iostream&) const;

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
