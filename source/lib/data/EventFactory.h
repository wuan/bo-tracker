#ifndef BLITZORTUNG_DATA_SAMPLEFACTORY_H_
#define BLITZORTUNG_DATA_SAMPLEFACTORY_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/Event.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for gps information
    class EventFactory : boost::noncopyable {

      public:

	typedef std::auto_ptr<EventFactory> AP;

	virtual Event::AP createEvent(Waveform::AP wfm, GpsInfo::AP gpsInfo) const = 0;

	virtual Event::AP createEvent(std::iostream& stream, const gr::date& date) const = 0;
    };

  }
}
#endif
