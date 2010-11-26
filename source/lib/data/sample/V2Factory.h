#ifndef BLITZORTUNG_DATA_SAMPLE_V2FACTORY_H_
#define BLITZORTUNG_DATA_SAMPLE_V2FACTORY_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/SampleFactory.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class for gps information
      class V2Factory : public SampleFactory {

	public:

	  virtual Sample::AP createSample(Sample::Waveform::AP wfm, GpsInfo::AP gpsInfo) const;

	  virtual Sample::AP createSample(std::iostream& stream, const gr::date& date) const;

      };

    }
  }
}
#endif
