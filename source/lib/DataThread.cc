
#include "DataThread.h"
#include "exception/Base.h"

namespace blitzortung {

  DataThread::DataThread(Queue<data::Event>& sampleQueue, EventCountBuffer& eventCountBuffer, network::transfer::Base& transfer, output::Base& output) :
    sampleQueue_(sampleQueue),
    eventCountBuffer_(eventCountBuffer),
    transfer_(transfer),
    events_(new data::Events()),
    output_(output),
    logger_("DataThread")
  {
    eventRateLimit_ = 1.0;

    if (logger_.isDebugEnabled())
      logger_.debugStream() << "initalized(eventRateLimit: " << eventRateLimit_ << ")";
  }

  DataThread::~DataThread() {
    if (logger_.isDebugEnabled())
      logger_.debugStream() << "deleted";
  }

  void DataThread::setEventRateLimit(const double eventRateLimit) {
    eventRateLimit_ = eventRateLimit;
  }

  data::Events::AP DataThread::prepareData() {
    data::Events::AP deletedEvents(new data::Events());

    double secondsElapsed = eventCountBuffer_.getActualSize();
    double eventRate = double(eventCountBuffer_.getSum()) / secondsElapsed;

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "prepareData() " << events_->size() << " events (rate " << eventRate << " events/second)";

    if (eventRate > eventRateLimit_) {
      events_->sort(data::Event::CompareAmplitude());

      int sampleLimit = eventRateLimit_ * secondsElapsed;

      deletedEvents->transfer(deletedEvents->end(), events_->begin(), events_->end(), *events_);

      if (logger_.isNoticeEnabled()) {
	logger_.noticeStream() << "prepareData() limit " << eventRateLimit_ << " reached, interval seconds: " << secondsElapsed << ", erasing to " << sampleLimit << " elements (new # of elements: " << events_->size() << ")";
      }

      // time sort events
      events_->sort();
    }

    return deletedEvents;
  }

  void DataThread::operator()() {

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "() started";

    while (true) {
      boost::xtime xt;
      boost::xtime_get(&xt, boost::TIME_UTC);
      xt.sec += 1;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "() wait for data";

      // wait for next incoming sample
      sampleQueue_.timed_wait(xt);

      bool sendAgain = false;
      do {
	// get new events from queue until it is empty
	while (! sampleQueue_.empty()) {

	  if (events_->size() != 0 && events_->getDate() != sampleQueue_.front().getWaveform().getTime().date()) {
	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "() stopped reading queue at transition to next day";
	    sendAgain = true;
	    break;
	  }

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() pop from queue " << sampleQueue_.front().getWaveform().getTime();

	  events_->add(sampleQueue_.pop());
	}

        eventCountBuffer_.add(events_->size());

	if (events_->size() > 0) {

	  if (logger_.isDebugEnabled())
	    logger_.debug("() transmitting/saving data");

	  // prepare data for transmission
	  data::Events::AP deletedEvents = prepareData();

	  // transmit data
	  transfer_.send(*events_);

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollect events " << events_->size() << " + " << deletedEvents->size();

	  events_->transfer(events_->end(), *deletedEvents);

	  events_->sort();

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "() recollected " << events_->size() << " events ";

	  // remove empty events from list
	  for (data::Event::VI event = events_->begin(); event != events_->end(); event++) {
	    if (event->getWaveform().isEmpty())
	      events_->erase(event--);
	  }

	  output_.output(*events_);

	  // delete all events
	  events_->clear();
	}

      } while (sendAgain);

    }

    if (logger_.isInfoEnabled())
      logger_.infoStream() << "() terminated";
  }

}
