
#include "DataThread.h"
#include "exception/Base.h"

namespace blitzortung {

  DataThread::DataThread(Queue<data::Event>& sampleQueue, const network::transfer::Base& transfer, output::Base& output)
    : sampleQueue_(sampleQueue),
    transfer_(transfer),
    events_(new data::Events()),
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

  data::Events::AP DataThread::prepareData(pt::ptime& now, pt::ptime& lastSent) {
    data::Events::AP deletedEvents(new data::Events());

    double secondsElapsed = ((now - lastSent).total_milliseconds() / 1000);
    double eventRate = double(events_->size()) / secondsElapsed;

    lastSent = now;

    if (logger_.isDebugEnabled())
      logger_.debugStream() << "prepareData() " << events_->size() << " events (rate " << eventRate << " events/second) at " << now;

    if (eventRate > eventRateLimit_) {
      events_->sort(data::Event::CompareAmplitude());

      int sampleLimit = eventRateLimit_ * secondsElapsed;

      //events_->transfer(events_->begin() + sampleLimit, events_->end());
      deletedEvents->transfer(deletedEvents->end(), events_->begin(), events_->end(), *events_);

      if (logger_.isInfoEnabled()) {
	logger_.infoStream() << "prepareData() limit " << eventRateLimit_ << " reached, interval seconds: " << secondsElapsed << ", erasing to " << sampleLimit << " elements (new # of elements: " << events_->size() << ")";
      }

      // time sort events
      events_->sort();
    }

    return deletedEvents;
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

      // get new events from queue until it is empty
      while (! sampleQueue_.empty()) {
	data::Event::AP sample(sampleQueue_.pop());

	if (events_->size() != 0 && events_->getDate() != sample->getWaveform().getTime().date()) {
	  sampleQueue_.push(sample);
	  break;
	}

	events_->add(sample);
      }

      pt::ptime now = pt::second_clock::universal_time();

      if (now - lastSent >= sleepTime_) {

	if (logger_.isDebugEnabled())
	  logger_.debug("() awake, transmitting data");

	if (events_->size() > 0) {

	  // prepare data for transmission
	  data::Events::AP deletedEvents = prepareData(now, lastSent);

	  logger_.infoStream() << "transmit " << events_->size() << " events";
	  // transmit data
	  transfer_.send(*events_);

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollect events " << events_->size() << " + " << deletedEvents->size();

	  events_->transfer(events_->end(), *deletedEvents);

	  events_->sort();

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollected " << events_->size() << " events ";

	  logger_.infoStream() << "output " << events_->size() << " events";
	  output_.output(*events_);

	  // delete all events
	  events_->clear();
	}

      }
    }
  }

}
