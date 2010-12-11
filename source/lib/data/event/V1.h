#ifndef BLITZORTUNG_DATA_SAMPLE_VERSION1_H_
#define BLITZORTUNG_DATA_SAMPLE_VERSION1_H_

#include "namespaces.h"
#include "Logger.h"
#include "data/Event.h"

namespace blitzortung {
  namespace data {
    namespace event {

      //! class V1 for event data version 1
      class V1 : public Event {
	
	public:

	  V1(Waveform::AP, GpsInfo::AP);

	  V1(std::iostream& stream, const gr::date& date);

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
