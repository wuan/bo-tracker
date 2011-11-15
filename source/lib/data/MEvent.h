#ifndef BLITZORTUNG_DATA_MEVENT_H_
#define BLITZORTUNG_DATA_MEVENT_H_

#include "namespaces.h"
#include "data/Event.h"

namespace blitzortung {
  namespace data {

    //! class for gps information
    class MEvent : public Event {

      public:

	typedef boost::ptr_vector<MEvent> V;
	typedef V::iterator VI;
	typedef V::const_iterator CVI;
	typedef std::shared_ptr<V> VP;
	typedef std::unique_ptr<MEvent> AP;

      private:

	const std::string rawData_;

      public:

	MEvent(Waveform::AP&&, GpsInfo::AP&&, const std::string&);

	virtual ~MEvent();

	//! get the raw data string
	const std::string& getRawData() const;
    };

  }
}
#endif
