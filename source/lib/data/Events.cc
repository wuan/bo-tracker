#include "data/Events.h"
#include "data/EventsFile.h"

namespace blitzortung {
  namespace data {

    Events::Events() :
      logger_("data.Events")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create() " << this;
    }

    Events::Events(const gr::date& date, const Format::CP& dataFormat) :
      logger_("data.Events")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create(" << date << ", " << dataFormat << ") " << this;
      date_ = date;
      dataFormat_ = dataFormat;
    }

    Events::Events(const EventsHeader& header) :
      logger_("data.Events")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create(header) " << this;
      dataFormat_ = header.getDataFormat();
      date_ = header.getDate();
    }

    Events::~Events() {
      if (logger_.isDebugEnabled())
	logger_.debug("destroy");
    }

    void Events::add(Event* event) {
      if (events_.size() == 0) {
	date_ = event->getWaveform().getTime().date();
	dataFormat_ = event->getWaveform().getArray().getFormat();
      } else {
	if (date_ != event->getWaveform().getTime().date())
	  throw exception::Base("Events::add() event date mismatch");
	if (*(dataFormat_) != *(event->getWaveform().getArray().getFormat()))
	  throw exception::Base("Events::add() event dataformat mismatch");
      }

      events_.push_back(event);
    }

    void Events::add(Event::AP event) {
      add(event.release());
    }

    void Events::add(Event::V::auto_type event) {
      add(event.release());
    }

    void Events::add(Events&) throw(exception::Base) {
    }

    const gr::date& Events::getDate() const {
      return date_;
    }

    void Events::setDate(const gr::date& date) {
      if (events_.size() == 0) {
	date_ = date;
      } else {
	throw exception::Base("data::Events::setDate() setting of date not allowed");
      }
    }

    const Format::CP& Events::getDataFormat() const {
      return dataFormat_;
    }


    int Events::size() const {
      return events_.size();
    }

    void Events::sort() {
      events_.sort();
    }

    void Events::clear() {
      events_.clear();
    }

    Event::VI Events::erase(Event::VI element) {
      return events_.erase(element);
    }

    Event::VI Events::erase(Event::VI start, Event::VI end) {
      return events_.erase(start, end);
    }

    const Event& Events::front() const {
      return events_.front();
    }

    Event& Events::front() {
      return events_.front();
    }

    const Event& Events::back() const {
      return events_.back();
    }

    Event& Events::back() {
      return events_.back();
    }

    Event::CVI Events::begin() const {
      return events_.begin();
    }

    Event::VI Events::begin() {
      return events_.begin();
    }

    Event::CVI Events::end() const {
      return events_.end();
    }

    Event::VI Events::end() {
      return events_.end();
    }

    void Events::transfer(Event::VI target, Event::VI begin, Event::VI end, Events& source) {
      events_.transfer(target, begin, end, source.events_);
    }

    void Events::transfer(Event::VI target, Events& source) {
      events_.transfer(target, source.events_);
    }

    Event::AP Events::release(Event::VI target) {
      return Event::AP(events_.release(target).release());
    }

    std::string Events::appendToFile(const std::string& fileName) const {
      if (events_.size() > 0) {

	if (logger_.isDebugEnabled()) {
	  logger_.debugStream() << "appendToFile() " << fileName << " (" << events_.size() << " events)";
	}

	EventsFile eventsFile(fileName);

	eventsFile.append(*this);
	
	return eventsFile.getFilename();
      }
      return "";
    }

    std::string Events::writeToFile(const std::string& fileName) {
      if (events_.size() > 0) {

	if (logger_.isDebugEnabled()) {
	  logger_.debugStream() << "appendToFile() " << fileName << " (" << events_.size() << " events)";
	}

	EventsFile eventsFile(fileName);

	eventsFile.write(*this);
	
	return eventsFile.getFilename();
      }
      return "";
    }

    void Events::readFromFile(const std::string& fileName, const pt::time_duration& startTime, const pt::time_duration& endTime) {

      if (logger_.isDebugEnabled()) {
	logger_.debugStream() << "readFromFile() " << fileName << " (from " << startTime << " to " << endTime <<")";
      }

      EventsFile eventsFile(fileName);
      replace(*(eventsFile.read(startTime, endTime)));
    }

    void Events::replace(Events& source) {

      date_ = source.date_;
      dataFormat_ = source.dataFormat_;

      events_.clear();
      events_.transfer(events_.begin(), source.events_);
    }


  }
}
