#ifndef BLITZORTUNG_NETWORK_BASE_H_
#define BLITZORTUNG_NETWORK_BASE_H_

#include <queue>

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "namespaces.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace network {

    template <typename T>
      class Queue {
	private:
	  std::queue<T> queue_;
	  mutable boost::mutex mutex_;
	  boost::condition condition_;

	public:
	  void push(const T& data) {
	    boost::mutex::scoped_lock lock(mutex_);

	    const bool emptyBeforePush = empty();

	    queue_.push(data);

	    if (emptyBeforePush)
	      condition_.notify_one();

	  }

	  bool empty() const {
	    boost::mutex::scoped_lock lock(mutex_);
	    return queue_.empty();
	  }

	  T& front() {
	    boost::mutex::scoped_lock lock(mutex_);
	    return queue_.front();
	  }

	  T const& front() const {
	    boost::mutex::scoped_lock lock(mutex_);
	    return queue_.front();
	  }

	  void wait() {
	    boost::mutex::scoped_lock lock(mutex_);

	    while (queue_.empty()) {
	      condition_.wait(lock);
	    }
	  }

	  void pop() {
	    boost::mutex::scoped_lock lock(mutex_);
	    queue_.pop();
	  }
      };


    class Base : private boost::noncopyable {

      private:

	class callable {
	  private:
	    Queue<data::sample::Base*>& sampleQueue_;
	    data::sample::Base::V samples_;
	    //data::sample::Base::VP samples(new data::sample::Base::V());

	  public:
	    callable(Queue<bo::data::sample::Base*>& sampleQueue)
	      : sampleQueue_(sampleQueue)
	    {
	    }

	    ~callable() {
	    }

	    void operator ()() {
	      std::cout << "running\n";
	    }
	};

	Queue<bo::data::sample::Base*> sampleQueue_;

      public:
	Base();

	virtual ~Base();

	//! add sample to send queue
	void put(std::auto_ptr<data::sample::Base>);

    };
  }
}

#endif
