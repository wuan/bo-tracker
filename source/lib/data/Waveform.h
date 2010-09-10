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

	const pt::ptime t0_;
	float maxVal_;
	int maxIndex_;
	std::vector<T> xdata_;
	std::vector<T> ydata_;

      public:

	Waveform(const pt::ptime&);

	virtual ~Waveform();

	//! add sample to collection
	void add(T, T);

	int getMaxIndex() const;

	T getMaxX() const;

	T getMaxY() const;

    };

  }
}
#endif
