#ifndef BLITZORTUNG_DATA_SAMPLE_H_
#define BLITZORTUNG_DATA_SAMPLE_H_

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "namespaces.h"
#include "data/Waveform.h"
#include "data/GpsInfo.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for gps information
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

      protected:

	Event(Waveform::AP, GpsInfo::AP);

	Event(std::iostream& stream, const gr::date& date, unsigned int size);

      public:

	//! get reference to waveform
	const Waveform& getWaveform() const;

	//! release waveform from sample
	Waveform::AP releaseWaveform();

	//! get reference to gps information
	const GpsInfo& getGpsInfo() const;

	//! release gps information from sample
	GpsInfo::AP releaseGpsInfo();

	//! comparison operator <
	bool operator<(const Event &) const;

	//! class to compare timestamp of Strokes
	struct CompareAmplitude : public std::binary_function<Event, Event, bool> {
	  bool operator()(const first_argument_type& x, const second_argument_type& y) const;
	};
	
	//! getter for sample format version
	virtual unsigned short getVersion() const = 0;

	//! getter for amount of samples in waveform
	virtual unsigned short getNumberOfEvents() const = 0;

	//! process sample to data save format
	virtual Waveform::AP processWaveform() const = 0;

	//! write binary object data to stream
	void toStream(std::iostream&) const;

	//! read binary object data from stream
	void fromStream(std::iostream&, const gr::date&);

	//! get binary storage size of sample
	unsigned int getSize() const;

    };

    std::ostream& operator << (std::ostream& os, const Event&);

  }
}
#endif
