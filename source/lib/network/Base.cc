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
	  std::cout << "network: running\n";

	  while (true) {
	    const boost::system_time timeout=boost::get_system_time() + boost::posix_time::milliseconds(500);
	    sampleQueue_.timed_wait(timeout);

	    if (! sampleQueue_.empty()) {
	      samples_->push_back(sampleQueue_.front());
	      sampleQueue_.pop();
	      std::cout << "network: # of samples " << samples_->size() << std::endl;
	    } else {
	      std::cout << "network: timeout\n";
	    }
	  }
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

