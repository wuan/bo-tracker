#ifndef BLITZORTUNG_DATA_WAVEFORM_H_
#define BLITZORTUNG_DATA_WAVEFORM_H_

#include <iostream>

#include "namespaces.h"
#include "data/ArrayOf.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    class Format;

    //! class for waveforms
    class Waveform : public virtual Array {

      public:

	typedef std::unique_ptr<Waveform> AP;

      public:

	//! delete waveform object
	virtual ~Waveform();

	//! return timestamp of waveform
	virtual const pt::ptime& getTime() const = 0;

	//! return time of sample with given index
	virtual pt::ptime getTime(unsigned int index) const = 0;

	//! return delta time of waveform
	virtual const pt::time_duration& getTimeDelta() const = 0;

	//! static function to determine size of a particular waveform structure
	static unsigned int GetSize(const data::Format& dataFormat);
    };

  }
}
#endif
