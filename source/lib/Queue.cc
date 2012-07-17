
#include "data/Event.h"
#include "Queue.h"

namespace blitzortung {

  template <typename T> Queue<T>::Queue(int timeout) :
    timeout_(timeout),
    queue_(),
    mutex_(),
    condition_(),
    logger_("Queue")
  {
  }

  template <typename T> Queue<T>::~Queue()
  {
    // TODO add code to free content of queue here
  }

  template <typename T> void Queue<T>::push(std::unique_ptr<T>&& data) {
    std::unique_lock<std::mutex> lock(mutex_);

    const bool emptyBeforePush = queue_.empty();

    queue_.push(data.release());

    if (emptyBeforePush)
      condition_.notify_one();
  }

  template <typename T> bool Queue<T>::empty() const {
    return queue_.empty();
  }

  template <typename T> T& Queue<T>::front() {
    return *(queue_.front());
  }

  template <typename T> T const& Queue<T>::front() const {
    return *(queue_.front());
  }

  template <typename T> void Queue<T>::wait() {
    std::unique_lock<std::mutex> lock(mutex_);

    while (queue_.empty()) {
      condition_.wait(lock);
    }
  }

  template <typename T> void Queue<T>::timed_wait(const std::chrono::seconds& duration) {
    std::unique_lock<std::mutex> lock(mutex_);

    condition_.wait_for(lock, duration);
  }

  template <typename T> std::unique_ptr<T> Queue<T>::pop() {
    std::unique_lock<std::mutex> lock(mutex_);

    std::unique_ptr<T> uniquePtr(queue_.front());

    queue_.pop();

    return uniquePtr;
  }

  //! explicit instatiation
  template class Queue<data::Event>;
}
