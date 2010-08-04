#include "network/Base.h"

namespace blitzortung {
  namespace network {

    Base::Base() {
    }

    Base::~Base() {
    }

    void Base::put(std::auto_ptr<data::sample::Base> sample) {
      sampleQueue_.push(sample.release());
    }

  }
}

