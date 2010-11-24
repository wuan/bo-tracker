#ifndef BLITZORTUNG_DATA_WAVEFORM_H_
#define BLITZORTUNG_DATA_WAVEFORM_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    template <typename T>
    class Waveform : boost::noncopyable {

      private:

	//! reference time
	pt::ptime t0_;

	//! time difference between samples in nanoseconds
	pt::time_duration dt_;
	
	//! maximum absolute value of signal
	float maxVal_;
	
	//! index of maximum absolute value of signal
	int maxIndex_;
	
	//! vector for x channel data
	std::vector<T> xdata_;
	
	//! vector for y channel data
	std::vector<T> ydata_;

      public:

        //! create a waveform object
	/*!
	\param t0 reference time of waveform
	*/
	Waveform(const pt::ptime& t0, const pt::time_duration& dt=pt::nanoseconds(0));

        //! create a waveform object from a stream
	/*!
	\param stream from which the waveform object should be read
	\param date of the stream 
	\param number of elements to read
	*/
	Waveform(std::iostream& stream, gr::date date, const unsigned int elements);

	//! delete waveform object
	virtual ~Waveform();

	//! add sample to collection
	/*!
	\param x x-value of sample
	\param y y-value of sample
	*/
	void add(T x, T y);

	//! returns x-value at index
	T getX(unsigned int index) const;

	//! returns y-value at index
	T getY(unsigned int index) const;

	//! returns amplitude at index
	float getAmplitude(unsigned int index) const;

	//! returns index of maximum value
	unsigned int getMaxIndex() const;

	//! returns x-value at maximum
	T getMaxX() const;

	//! returns y-value at maximum
	T getMaxY() const;

	//! return time of waveform or of sample with given index
	const pt::ptime& getTime() const;

	//! return time of waveform or of sample with given index
	pt::ptime getTime(unsigned int index) const;

	//! write to stream
	void write(std::iostream&, unsigned int elementCount);

	//! static function to determine size of a particular waveform structure
	static unsigned int GetSize(unsigned int elements);

	typedef std::auto_ptr<Waveform<T> >AP;
    };

  }
}
#endif
