#ifndef BLITZORTUNG_DATA_SAMPLE_V2FACTORY_H_
#define BLITZORTUNG_DATA_SAMPLE_V2FACTORY_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/EventFactory.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class for gps information
      class V2Factory : public EventFactory {

	public:

	  virtual Event::AP createEvent(Waveform::AP wfm, GpsInfo::AP gpsInfo) const;

	  virtual Event::AP createEvent(std::iostream& stream, const gr::date& date) const;

      };

    }
  }
}
#endif
