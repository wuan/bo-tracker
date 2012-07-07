#ifndef BLITZORTUNG_DATA_EVENT_H_
#define BLITZORTUNG_DATA_EVENT_H_

#include <boost/ptr_container/ptr_vector.hpp>
#include <json/json.h>

#include "namespaces.h"
#include "data/Waveform.h"
#include "data/GpsInfo.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class representing a single recorded sferics event with all relevant information
    class Event {

      public:

	typedef boost::ptr_vector<Event> V;
	typedef V::iterator VI;
	typedef V::const_iterator CVI;
	typedef std::shared_ptr<V> VP;
	typedef std::unique_ptr<Event> AP;

      private:

	//! recorded waveform
	Waveform::AP waveform_;

	//! corresponding gps information
	GpsInfo::AP gpsInfo_;

      public:

        //! construct Event with Data
	Event(Waveform::AP&&, GpsInfo::AP&&);

	//! construct Event from stream 
	Event(const data::Format& dataFormat, const gr::date& date, std::iostream& stream);

	//! destruct Event
	virtual ~Event();

	Event(const Event&) = delete;
	Event& operator=(const Event&) = delete;

	//! return reference to sample waveform
	const Waveform& getWaveform() const;

	//! release waveform from sample object
	Waveform::AP releaseWaveform();

	//! return reference to sample gps information
	const GpsInfo& getGpsInfo() const;

	//! release gps information from sample object
	GpsInfo::AP releaseGpsInfo();

	//! comparison operator <
	bool operator<(const Event &) const;

	//! comparison operator ==
	bool operator==(const Event &) const;

	//! class used to sort events by their maximum amplitude
	struct CompareAmplitude : public std::binary_function<Event, Event, bool> {
	  bool operator()(const first_argument_type& x, const second_argument_type& y) const;
	};
	
	//! write object as binary data to stream
	void toStream(std::iostream&) const;

	//! get timestamp as string
	const pt::ptime& getTimestamp() const;

	//! get timestamp as string
	pt::ptime getTimestamp(unsigned int index) const;

	//! get binary storage size of sample
	size_t getStorageSize() const;

	//! determine storage size of event in bytes
	static size_t GetSize(const Format& dataFormat);

	//! return as json object
	json_object* asJson() const;
    };

    std::ostream& operator<< (std::ostream& os, const Event&);

  }
}
#endif
