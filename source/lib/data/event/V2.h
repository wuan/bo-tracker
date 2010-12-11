#ifndef BLITZORTUNG_DATA_EVENT_VERSION2_H_
#define BLITZORTUNG_DATA_EVENT_VERSION2_H_

#include "namespaces.h"
#include "Logger.h"
#include "data/Event.h"

namespace blitzortung {
  namespace data {
    namespace event {

      //! class V2 for event data version 2
      class V2 : public Event {
	
	public:

	  V2(Waveform::AP, GpsInfo::AP);

	  V2(std::iostream& stream, const gr::date& date);

	  //! getter for format version
	  virtual unsigned short getVersion() const;

	  //! getter for number of events in waveform
	  unsigned short getNumberOfEvents() const;

	  //! perform special processing befor saving data
	  Waveform::AP processWaveform() const;

      };

    }
  }
}

#endif
