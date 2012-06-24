#ifndef BLITZORTUNG_DATA_WAVEFORM_H_
#define BLITZORTUNG_DATA_WAVEFORM_H_

#include <iostream>

#include <json/json.h>

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
	virtual const pt::ptime& getTimestamp() const = 0;

	//! return time of sample with given index
	virtual pt::ptime getTimestamp(unsigned int index) const = 0;

	//! return delta time of waveform
	virtual const pt::time_duration& getTimeDelta() const = 0;

	//! returns the storage size of the array in bytes
	virtual unsigned int getStorageSize() const = 0;

	//! write binary object data to stream
	virtual void toStream(std::iostream&) const = 0;

	//! return json object representation
	virtual json_object* asJson(bool normalize=true) const = 0;
	
	//! static function to determine size of a particular waveform structure
	static unsigned int GetSize(const data::Format& dataFormat);
    };

    std::ostream& operator <<(std::ostream& os, const bo::data::Waveform& wfm);
  }
}
#endif
