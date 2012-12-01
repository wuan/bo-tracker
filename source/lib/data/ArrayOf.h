#ifndef BLITZORTUNG_DATA_ARRAYOF_H_
#define BLITZORTUNG_DATA_ARRAYOF_H_

#include "data/Array.h"

namespace blitzortung {
  namespace data {

    //! class for waveform arrays
    template<typename T, int numberOfSamples, int numberOfChannels>
    class ArrayOf : public virtual Array {

      public:
	typedef std::unique_ptr<ArrayOf> AP;

      private:
      
        std::vector<T> data_;

      protected:

	//! returns the size of a single data element
	unsigned char getElementSize() const;

      public:

        //! create a waveform array object
	/*!
	\param format reference to array format definition
	*/
	ArrayOf();

	//! delete waveform object
	virtual ~ArrayOf();

	//! returns the data format of the current array
	Format getFormat() const;

	//! returns the number of measurements per waveform
	unsigned int getNumberOfSamples() const;

	//! returns the number of channels per measurement
	unsigned short getNumberOfChannels() const;
	
	//! returns true if the waveform array is empty
	bool isEmpty() const;

	//! set a particular sample in a given channel to a value
	void set(T value, unsigned int sample, unsigned short channel=0);

	//! set a particular sample in a given channel to a value
	void setInt(int value, unsigned int sample, unsigned short channel=0);
	
	//! set a particular sample in a given channel to a value
	void setFloat(float value, unsigned int sample, unsigned short channel=0);

	//! return the value of a sample in a given channel
	T get(unsigned int sample, unsigned short channel=0) const;

	//! return the value of a sample in a given channel
	int getInt(unsigned int sample, unsigned short channel=0) const;

	//! return the value of a sample in a given channel
	float getFloat(unsigned int sample, unsigned short channel=0) const;

	//! returns the storage size of the array in bytes
	virtual unsigned int getStorageSize() const;

	//! write binary object data to stream
	virtual void toStream(std::iostream&) const;

	//! read binary object data from stream
	virtual void fromStream(std::iostream&); 

    };

    template<typename T, int numberOfSamples, int numberOfChannels>
    std::ostream& operator <<(std::ostream& os, const ArrayOf<T, numberOfSamples, numberOfChannels>&);

  }
}
#endif
