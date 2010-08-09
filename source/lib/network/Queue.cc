
#include "data/sample/Base.h"
#include "network/Queue.h"

namespace blitzortung {
  namespace network {

    template <typename T> Queue<T>::Queue(int timeout) :
      timeout_(timeout)
    {
    }

    template <typename T> void Queue<T>::push(const T& data) {
      boost::mutex::scoped_lock lock(mutex_);

      const bool emptyBeforePush = queue_.empty();

      queue_.push(data);

      if (emptyBeforePush)
	condition_.notify_one();
    }

    template <typename T> bool Queue<T>::empty() const {
      boost::mutex::scoped_lock lock(mutex_);

      return queue_.empty();
    }

    template <typename T> T& Queue<T>::front() {
      boost::mutex::scoped_lock lock(mutex_);

      return queue_.front();
    }

    template <typename T> T const& Queue<T>::front() const {
      boost::mutex::scoped_lock lock(mutex_);
      return queue_.front();
    }

    template <typename T> void Queue<T>::wait() {
      boost::mutex::scoped_lock lock(mutex_);

      while (queue_.empty()) {
	condition_.wait(lock);
      }
    }

    template <typename T> void Queue<T>::timed_wait(const boost::xtime& xtime) {
      boost::mutex::scoped_lock lock(mutex_);

      condition_.timed_wait(lock, xtime);
    }

    template <typename T> void Queue<T>::pop() {
      boost::mutex::scoped_lock lock(mutex_);
      queue_.pop();
    }

    //! explicit instatiation
    template class Queue<data::sample::Base*>;
  }
}
