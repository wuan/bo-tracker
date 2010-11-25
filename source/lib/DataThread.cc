
#include "DataThread.h"
#include "exception/Base.h"

namespace blitzortung {

  DataThread::DataThread(Queue<data::Sample>& sampleQueue, const network::transfer::Base& transfer, output::Base& output)
    : sampleQueue_(sampleQueue),
    transfer_(transfer),
    samples_(new data::Sample::V()),
    output_(output),
    logger_("DataThread")
  {
    sleepTime_ = pt::seconds(20);
    eventRateLimit_ = 1.0;

    if (logger_.isDebugEnabled())
      logger_.debugStream() << "initalized( sleep: " << sleepTime_ << ", eventRateLimit: " << eventRateLimit_ << ")";
  }

  DataThread::~DataThread() {
    if (logger_.isDebugEnabled())
      logger_.debugStream() << "deleted";
  }

  void DataThread::setSleepTime(const pt::time_duration& sleepTime) {
    sleepTime_ = sleepTime;
  }

  void DataThread::setEventRateLimit(const double eventRateLimit) {
    eventRateLimit_ = eventRateLimit;
  }

  data::Sample::VP DataThread::prepareData(pt::ptime& now, pt::ptime& lastSent) {
    data::Sample::VP deletedSamples(new data::Sample::V());

    if (logger_.isDebugEnabled())
      logger_.debugStream() << "() sample vector contains " << samples_->size() << " elements";

    double secondsElapsed = ((now - lastSent).total_milliseconds() / 1000);
    double eventRate = double(samples_->size()) / secondsElapsed;

    lastSent = now;

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "() sending " << samples_->size() << " samples (rate " << eventRate << " samples/second) at " << now;

    if (eventRate > eventRateLimit_) {
      samples_->sort(data::Sample::CompareAmplitude());

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "() ratelimit " << eventRateLimit_ << " reached, interval seconds: " << secondsElapsed;

      int sampleLimit = eventRateLimit_ * secondsElapsed;

      //samples_->transfer(samples_->begin() + sampleLimit, samples_->end());
      deletedSamples->transfer(deletedSamples->end(), samples_->begin(), samples_->end(), *samples_);

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "() erasing elements to have " << sampleLimit << " elements (new # of elements: " << samples_->size() << ")";

      // time sort samples
      samples_->sort();
    }

    return deletedSamples;
  }



  void DataThread::operator()() {

    pt::ptime lastSent = pt::second_clock::universal_time();

    while (true) {
      boost::xtime xt;
      boost::xtime_get(&xt, boost::TIME_UTC);
      xt.sec += 1;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() wait for data";

      sampleQueue_.timed_wait(xt);

      // get new samples from queue until it is empty
      while (! sampleQueue_.empty()) {
	data::Sample::AP sample(sampleQueue_.pop());

	samples_->push_back(sample);
      }

      pt::ptime now = pt::second_clock::universal_time();

      if (now - lastSent >= sleepTime_) {

	if (samples_->size() > 0) {

	  // prepare data for transmission
	  data::Sample::VP deletedSamples = prepareData(now, lastSent);

	  // transmit data
	  transfer_.send(samples_);

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollect samples " << samples_->size() << " + " << deletedSamples->size();
	  samples_->transfer(samples_->end(), *deletedSamples);

	  samples_->sort();

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollected " << samples_->size() << " samples ";

	  output_.output(samples_);

	  // delete all samples
	  samples_->clear();
	}

      }
    }
  }

}
