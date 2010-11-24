#include "data/sample/V1.h"
#include "util/Stream.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      V1::V1()
      {
      }

      V1::~V1() {
      }

      Base* V1::Creator::operator()() const {
	return new V1();
      }

      unsigned short V1::getVersion() const {
	return 1;
      }

      unsigned short V1::getNumberOfSamples() const {
        return 1;
      }

    }
  }
}
