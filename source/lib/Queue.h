#ifndef BLITZORTUNG_NETWORK_QUEUE_H_
#define BLITZORTUNG_NETWORK_QUEUE_H_

#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "Logger.h"

namespace blitzortung {

  template <typename T>
    class Queue {

      private:
	//! wait timeout in seconds
	int timeout_;

	//! internal queue structure to store data
	std::queue<T*> queue_;

	//! mutex to ensure exclusive access to queue
	mutable std::mutex mutex_;

	//! condition to enable waiting on queue
	std::condition_variable condition_;

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
	void timed_wait(const std::chrono::seconds& duration);

	//! retrieve the first element of the queue
	std::unique_ptr<T> pop();

      private:

	Queue(const Queue&) = delete;
	Queue& operator=(const Queue&) = delete;
    };

}

#endif
