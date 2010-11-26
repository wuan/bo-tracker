#ifndef BLITZORTUNG_DATA_SAMPLEFACTORY_H_
#define BLITZORTUNG_DATA_SAMPLEFACTORY_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/Sample.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for gps information
    class SampleFactory : boost::noncopyable {

      public:

	typedef std::auto_ptr<SampleFactory> AP;

	virtual Sample::AP createSample(Sample::Waveform::AP wfm, GpsInfo::AP gpsInfo) const = 0;

	virtual Sample::AP createSample(std::iostream& stream, const gr::date& date) const = 0;
    };

  }
}
#endif
