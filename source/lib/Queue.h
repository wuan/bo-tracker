#ifndef BLITZORTUNG_NETWORK_QUEUE_H_
#define BLITZORTUNG_NETWORK_QUEUE_H_

#include <queue>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "Logger.h"

namespace blitzortung {

  template <typename T>
    class Queue : private boost::noncopyable {

      private:
	//! wait timeout in seconds
	int timeout_;

	//! internal queue structure to store data
	std::queue<T*> queue_;

	//! mutex to ensure exclusive access to queue
	mutable boost::mutex mutex_;

	//! condition to enable waiting on queue
	boost::condition condition_;

	//! logger for this class
	mutable Logger logger_;

      public:

	//! construct queue
	Queue(int timeout=0);

	//! destruct queue
	virtual ~Queue();

	//! push one element to the end of the queue
	void push(std::unique_ptr<T>&& data);

	//! check if the queue is empty
	bool empty() const;

	//! get a reference to the first element of the queue
	T& front();

	//! get a const reference to the first element of the queue
	T const& front() const;

	//! wait until an element is pushed to the end of the queue
	void wait();

	//! wait with timeout until an element is pushed to the end of the queue
	void timed_wait(const boost::xtime& xtime);

	//! retrieve the first element of the queue
	std::unique_ptr<T> pop();
    };

}

#endif
