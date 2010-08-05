#include "network/Base.h"

namespace blitzortung {
  namespace network {

    Base::Base() {
      callable networkThread(sampleQueue_);

      boost::thread thread(networkThread);

    }

    Base::~Base() {
    }

    void Base::put(std::auto_ptr<data::sample::Base> sample) {
      sampleQueue_.push(sample.release());
    }

  }
}

