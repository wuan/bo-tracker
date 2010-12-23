#ifndef BLITZORTUNG_DATA_WAVEFORM_H_
#define BLITZORTUNG_DATA_WAVEFORM_H_

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "Array.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    class Waveform : boost::noncopyable {

      public:

	typedef std::auto_ptr<Waveform> AP;

      private:

	//! reference time
	pt::ptime t0_;

	//! time difference between samples in nanoseconds
	pt::time_duration dt_;
	
	//! maximum absolute value of signal
	float maxVal_;
	
	//! index of maximum absolute value of signal
	int maxIndex_;

	Array::AP array_;

      public:

        //! create a waveform object
	/*!
	\param t0 reference time of waveform
	\param dt time between consecutive data
	*/
	Waveform(data::Array::AP array, const pt::ptime& t0, const pt::time_duration& dt=pt::nanoseconds(0));

        //! create a waveform object from a stream
	/*!
	\param stream from which the waveform object should be read
	\param date of the stream 
	\param elements number of elements to read
	*/
	Waveform(const data::Format& dataFormat, gr::date date, std::iostream& stream);

	//! delete waveform object
	virtual ~Waveform();

	//! add sample to collection
	/*!
	\param x x-value of sample
	*/
	void add(unsigned char x);

	//! returns value at index
	int get(unsigned int index, unsigned short channel=0) const;

	//! returns amplitude at index
	float getAmplitude(unsigned int index) const;

	//! returns index of maximum value
	unsigned int getMaxIndex() const;

	//! return timestamp of waveform
	const pt::ptime& getTime() const;

	//! return time of sample with given index
	pt::ptime getTime(unsigned int index) const;

	//! return delta time of waveform
	const pt::time_duration& getTimeDelta() const;

	//! return actual size of waveform
	unsigned int getNumberOfSamples() const;

	const Array& getArray() const {
	  return *array_;
	}

	//! write to stream
	void toStream(std::iostream&);

	//! static function to determine size of a particular waveform structure
	static unsigned int GetSize(const data::Format& dataFormat);
    };

    std::ostream& operator <<(std::ostream& os, const bo::data::Waveform& wfm);

  }
}
#endif
