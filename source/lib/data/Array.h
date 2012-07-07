#ifndef BLITZORTUNG_DATA_ARRAY_H_
#define BLITZORTUNG_DATA_ARRAY_H_

#include "namespaces.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    class Format;

    //! class for waveform arrays
    class Array {

      public:

	typedef std::unique_ptr<Array> AP;

      public:

	Array() = default;

	//! delete waveform object
	virtual ~Array();

	Array(const Array&) = delete;
	Array& operator=(const Array&) = delete;

	//! returns the data format of the current array
	virtual Format getFormat() const = 0;

	//! returns the size of a single data element
	virtual unsigned char getElementSize() const = 0;

	//! returns the number of measurements per waveform
	virtual unsigned int getNumberOfSamples() const = 0;

	//! returns the number of channels per measurement
	virtual unsigned short getNumberOfChannels() const = 0;
	
	//! returns true if the waveform array is empty
	virtual bool isEmpty() const = 0;

	//! set a particular sample in a given channel to a value
	virtual void setInt(int value, unsigned int sample, unsigned short channel=0) = 0;

	//! set a particular sample in a given channel to a value
	virtual void setFloat(float value, unsigned int sample, unsigned short channel=0) = 0;

	//! return the value of a sample in a given channel
	virtual int getInt(unsigned int sample, unsigned short channel=0) const = 0;

	//! return the value of a sample in a given channel
	virtual float getFloat(unsigned int sample, unsigned short channel=0) const = 0;

	//! returns amplitude at index
	float getAmplitude(unsigned int index) const;

	//! returns amplitude at index
	float getAmplitudeSquare(unsigned int index) const;

	//! returns amplitude at index
	float getPhase(unsigned int index) const;

	//! returns index of maximum value
	unsigned int getMaxIndex() const;

	//! returns index of maximum value ignoring clipped amplitudes
	unsigned int getMaxIndexNoClip() const;

	//! returns the storage size of the array in bytes
	virtual unsigned int getStorageSize() const = 0;

	//! write binary object data to stream
	virtual void toStream(std::iostream&) const = 0;

	//! read binary object data from stream
	virtual void fromStream(std::iostream&) = 0; 

      private:
	
	//! returns index of maximum value
	unsigned int getMaxIndexInternal(bool ignoreClip=false) const;

    };

  }
}
#endif
