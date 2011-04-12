#ifndef BLITZORTUNG_DATA_EVENT_H_
#define BLITZORTUNG_DATA_EVENT_H_

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "namespaces.h"
#include "data/Waveform.h"
#include "data/GpsInfo.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class representing a single recorded sferics event with all relevant information
    class Event : boost::noncopyable {

      public:

	typedef boost::ptr_vector<Event> V;
	typedef V::iterator VI;
	typedef V::const_iterator CVI;
	typedef boost::shared_ptr<V> VP;
	typedef std::auto_ptr<Event> AP;

      private:

	//! recorded waveform
	Waveform::AP waveform_;

	//! corresponding gps information
	GpsInfo::AP gpsInfo_;

      public:

        //! construct Event with Data
	Event(Waveform::AP, GpsInfo::AP);

	//! construct Event from stream 
	Event(data::Format::CP dataFormat, const gr::date& date, std::iostream& stream);

	//! destruct Event
	virtual ~Event();

	//! return reference to sample waveform
	const Waveform& getWaveform() const;

	//! release waveform from sample object
	Waveform::AP releaseWaveform();

	//! return reference to sample gps information
	const GpsInfo& getGpsInfo() const;

	//! release gps information from sample object
	GpsInfo::AP releaseGpsInfo();

	//! comparison operator <, used for time sorting of events
	bool operator<(const Event &) const;

	//! class used to sort events by their maximum amplitude
	struct CompareAmplitude : public std::binary_function<Event, Event, bool> {
	  bool operator()(const first_argument_type& x, const second_argument_type& y) const;
	};
	
	//! getter for number of samples in waveform
	virtual unsigned short getNumberOfSamples() const;

	//! write object as binary data to stream
	void toStream(std::iostream&) const;

	//! get binary storage size of sample
	unsigned int getSize() const;

	//! determine storage size of event in bytes
	static unsigned int GetSize(const Format::CP& dataFormat);

    };

    std::ostream& operator<< (std::ostream& os, const Event&);

  }
}
#endif
