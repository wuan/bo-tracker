#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      Base::Base() {
	// initialize data pointer as empty
	data_ = 0;
      }

      Base::~Base() {
	if (data_ != 0) {
	  // no problem, because we are noncopyable
	  delete data_;
	}
      }

      double Base::getAmplitude(int index) const {
	double xamp = getXAmplitude(index);
	double yamp = getYAmplitude(index);

	return sqrt(xamp * xamp + yamp * yamp);
      }
    }
  }
}
