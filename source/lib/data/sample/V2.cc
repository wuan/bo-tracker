#include "data/sample/V2.h"
#include "util/Stream.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      V2::V2()
      {
      }

      V2::~V2() {
      }

      Base* V2::Creator::operator()() const {
	return new V2();
      }

      unsigned short V2::getVersion() const {
	return 2;
      }

      unsigned short V2::getNumberOfSamples() const {
        return 128;
      }
    }
  }
}
