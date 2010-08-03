#ifndef BLITZORTUNG_NETWORK_BASE_H_
#define BLITZORTUNG_NETWORK_BASE_H_

#include <queue>

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "namespaces.h"

namespace blitzortung {
  namespace network {

    template <typename T>
      class BlockingQueue {
	private:
	  std::queue<T> queue_;
	  mutable boost::mutex mutex_;
	  boost::condition condition_;

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

    void transfer(std::auto_ptr<bo::data::sample::Base> sample) {
      std::cout << sample->getTime() << " " << sample->getAntennaLongitude() << " " << sample->getAntennaLatitude() << " " << (int) sample->getGpsNumberOfSatellites() << std::endl;
      for (int peak=1; peak<=1; peak++) {
	std::cout << "  " << peak << " " << sample->getTime(peak) << " " << sample->getAmplitude(peak) << " (" << sample->getXAmplitude(peak) << ", " << sample->getYAmplitude(peak) << ")" << std::endl;
      }
    }

    class Base : private boost::noncopyable {

      Base();

      virtual ~Base();

      //! add sample to send queue
      void put(std::auto_ptr<data::sample::Base>);

    }
  }
}
