#ifndef BLITZORTUNG_DATA_WAVEFORMOF_H_
#define BLITZORTUNG_DATA_WAVEFORMOF_H_

#include <iostream>

#include "namespaces.h"
#include "data/ArrayOf.h"
#include "data/Waveform.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for waveforms
    template<typename T, int numberOfSamples, int numberOfChannels>
    class WaveformOf : public Waveform, public ArrayOf<T, numberOfSamples, numberOfChannels> {

      public:

	typedef std::unique_ptr<WaveformOf<T, numberOfSamples, numberOfChannels>> AP;

      private:

	//! reference time
	pt::ptime t0_;

	//! time difference between samples in nanoseconds
	pt::time_duration dt_;

      public:

        //! create a waveform object
	/*!
	\param array unique_ptr to Array object
	\param t0 reference time of waveform
	\param dt time between consecutive data (non zero if Array has more than one sample
	*/
	WaveformOf(const pt::ptime& t0, const pt::time_duration& dt=pt::nanoseconds(0));

        //! create a waveform object from a stream
	/*!
	\param stream from which the waveform object should be read
	\param date of the stream 
	\param elements number of elements to read
	*/
	WaveformOf(const gr::date& date, std::iostream& stream);

	//! delete waveform object
	virtual ~WaveformOf();

	//! return timestamp of waveform
	const pt::ptime& getTimestamp() const;

	//! return time of sample with given index
	pt::ptime getTimestamp(unsigned int index) const;

	//! return delta time of waveform
	const pt::time_duration& getTimeDelta() const;

	//! returns the storage size of the array in bytes
	virtual unsigned int getStorageSize() const;

	//! write to stream
	virtual void toStream(std::iostream&) const;

	//! return json object representation
	json_object* asJson(bool normalize=false) const;
	
	//! static function to determine size of a particular waveform structure
	static unsigned int GetSize(const data::Format& dataFormat);
    };

    template<typename T, int numberOfSamples, int numberOfChannels>
    std::ostream& operator <<(std::ostream& os, const bo::data::WaveformOf<T, numberOfSamples, numberOfChannels>& wfm);

  }
}
#endif
