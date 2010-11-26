#ifndef BLITZORTUNG_DATA_SAMPLE_VERSION1_H_
#define BLITZORTUNG_DATA_SAMPLE_VERSION1_H_

#include "namespaces.h"
#include "Logger.h"
#include "data/Sample.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class V1 for sample data version 1
      class V1 : public Sample {
	
	public:

	  V1(Sample::Waveform::AP, GpsInfo::AP);

	  V1(std::iostream& stream, const gr::date& date);

	  //! getter for format version
	  virtual unsigned short getVersion() const;
	  
      	  //! getter for number of samples in waveform
	  unsigned short getNumberOfSamples() const;

	  //! perform special processing befor saving data
	  Sample::Waveform::AP processWaveform() const;

      };

    }
  }
}

#endif
