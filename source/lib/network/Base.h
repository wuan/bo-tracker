#ifndef BLITZORTUNG_NETWORK_BASE_H_
#define BLITZORTUNG_NETWORK_BASE_H_

#include <queue>

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/condition.hpp>

#include "namespaces.h"
#include "network/Creds.h"
#include "data/sample/Base.h"


namespace blitzortung {
  namespace network {

    template <typename T>
      class Queue {
	private:
	  int timeout_;
	  std::queue<T> queue_;
	  mutable boost::mutex mutex_;
	  boost::condition condition_;

	public:

	  Queue(int timeout=0) :
	    timeout_(timeout)
	  {
	  }

	  void push(const T& data) {
	    boost::mutex::scoped_lock lock(mutex_);

	    const bool emptyBeforePush = queue_.empty();

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

	  void timed_wait(const boost::xtime& xtime) {
	    boost::mutex::scoped_lock lock(mutex_);

	    condition_.timed_wait(lock, xtime);
	  }

	  void pop() {
	    boost::mutex::scoped_lock lock(mutex_);
	    queue_.pop();
	  }
      };



    class Base : private boost::noncopyable {

      private:
	Queue<bo::data::sample::Base*> sampleQueue_;

      public:
	Base(const Creds& creds, const int sleepTime, const double eventRateLimit);

	virtual ~Base();

	//! add sample to send queue
	void put(std::auto_ptr<data::sample::Base>);
    };
  }
}

#endif
