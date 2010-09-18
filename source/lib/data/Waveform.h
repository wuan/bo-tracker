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
	const pt::ptime t0_;
	
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
	Waveform(const pt::ptime& t0);

	//! delete waveform object
	virtual ~Waveform();

	//! add sample to collection
	/*!
	\param x x-value of sample
	\param y y-value of sample
	*/
	void add(T x, T y);

	//! returns index of maximum value
	int getMaxIndex() const;

	//! returns x-value at maximum
	T getMaxX() const;

	//! returns y-value at maximum
	T getMaxY() const;

    };

  }
}
#endif
