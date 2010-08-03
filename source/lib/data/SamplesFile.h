#ifndef BLITZORTUNG_DATA_SAMPLESFILE_H_
#define BLITZORTUNG_DATA_SAMPLESFILE_H_

#include "namespaces.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {

    class SamplesFile {

      private:
	readHeader();

      public:
	std::auto_ptr<sample::Base> readEntry();
    };

  }
}

#endif

