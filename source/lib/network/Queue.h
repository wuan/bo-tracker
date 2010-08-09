#ifndef BLITZORTUNG_NETWORK_QUEUE_H_
#define BLITZORTUNG_NETWORK_QUEUE_H_

#include <queue>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

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

	  Queue(int timeout=0);

	  void push(const T& data);

	  bool empty() const;

	  T& front();

	  T const& front() const;

	  void wait();

	  void timed_wait(const boost::xtime& xtime);

	  void pop();
      };

  }
}

#endif
