#include "network/Base.h"

namespace blitzortung {
  namespace network {

    class NetworkTransfer {
      private:
	Queue<data::sample::Base*>& sampleQueue_;
        data::sample::Base::VP samples_;

      public:
	NetworkTransfer(Queue<bo::data::sample::Base*>& sampleQueue)
	  : sampleQueue_(sampleQueue),
	  samples_(new data::sample::Base::V())
	{
	}

	virtual ~NetworkTransfer() {
	}

	void operator ()() {
	  std::cout << "running\n";
	  sampleQueue_.wait();
	}
    };

    Base::Base() {
      NetworkTransfer networkTransfer(sampleQueue_);

      boost::thread thread(networkTransfer);

    }

    Base::~Base() {
    }

    void Base::put(std::auto_ptr<data::sample::Base> sample) {
      sampleQueue_.push(sample.release());
    }

  }
}

