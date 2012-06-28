
#include "DataWorker.h"
#include "exception/Base.h"

namespace blitzortung {

  DataWorker::DataWorker(Queue<data::Event>& sampleQueue, EventCountBuffer& eventCountBuffer, network::transfer::Base& transfer, output::Base& output) :
    sampleQueue_(sampleQueue),
    eventCountBuffer_(eventCountBuffer),
    transfer_(transfer),
    events_(new data::Events()),
    output_(output),
    logger_("DataWorker")
  {
    eventRateLimit_ = 1.0;

    if (logger_.isDebugEnabled())
      logger_.debugStream() << "initalized(eventRateLimit: " << eventRateLimit_ << ")";
  }

  void DataWorker::setEventRateLimit(const double eventRateLimit) {
    eventRateLimit_ = eventRateLimit;
  }

  data::Events::AP DataWorker::prepareData(int eventsPerSecond) {
    data::Events::AP deletedEvents(new data::Events());

    double secondsElapsed = eventCountBuffer_.getActualSize();
    double eventRate = double(eventCountBuffer_.getSum() + eventsPerSecond) / (secondsElapsed + 1);

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "prepareData() " << events_->size() << " events (rate " << eventRate << " events/second)";

    if (eventRate > eventRateLimit_) {
      events_->sort(data::Event::CompareAmplitude());

      int sampleLimit = eventRateLimit_ * secondsElapsed;

      deletedEvents->transfer(deletedEvents->end(), events_->begin(), events_->end(), *events_);

      logger_.warnStream() << "prepareData() limit " << eventRateLimit_ << " reached, interval seconds: " << secondsElapsed << ", erasing to " << sampleLimit << " elements (new # of elements: " << events_->size() << ")" << " deleted # " << deletedEvents->size();

      // time sort events
      events_->sort();
    }

    return deletedEvents;
  }

  void DataWorker::operator()() {

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "() started";

    pt::ptime second(std::move(getSecond()));
    int eventsPerSecond = 0;

    while (true) {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() wait for data";

      // wait for next incoming sample
      sampleQueue_.timed_wait(std::chrono::seconds(1));

      bool sendAgain = false;
      do {
	// get new events from queue until it is empty
	while (! sampleQueue_.empty()) {

	  if (events_->size() != 0 && events_->getDate() != sampleQueue_.front().getWaveform().getTimestamp().date()) {
	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() stopped reading queue at transition to next day";
	    sendAgain = true;
	    break;
	  }

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() pop sample from queue " << sampleQueue_.front().getWaveform().getTimestamp();

	  auto event = std::move(sampleQueue_.pop());
	  events_->add(std::move(event));
	}

	pt::ptime currentSecond(std::move(getSecond()));
	if (currentSecond > second) {
	  // record number of events for the actual second
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << second << " : " << eventsPerSecond << " events per second";
          eventCountBuffer_.add(eventsPerSecond);
	  eventsPerSecond = 0;
	  second = currentSecond;
	}

	if (events_->size() > 0) {

	  if (logger_.isDebugEnabled())
	    logger_.debug("() transmitting/saving data");

	  // prepare data for transmission
	  data::Events::AP deletedEvents = prepareData(eventsPerSecond);

	  // transmit data
	  transfer_.send(*events_);

	  // only count transmitted events
	  eventsPerSecond += events_->size();

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollect events " << events_->size() << " + " << deletedEvents->size();

	  events_->transfer(events_->end(), *deletedEvents);

	  events_->sort();

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollected " << events_->size() << " events ";

	  // remove empty events from list
	  for (data::Event::VI event = events_->begin(); event != events_->end(); event++) {
	    if (event->getWaveform().isEmpty()) {
	      events_->erase(event--);
	    }
	  }

	  output_.output(*events_);
	}

	// delete all events
	events_->clear();

      } while (sendAgain);

    }

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "() terminated";
  }

  pt::ptime DataWorker::getSecond() const {
    return pt::second_clock::universal_time();
  }

}
