#include "data/Events.h"
#include "data/EventsFile.h"

namespace blitzortung {
  namespace data {

    Events::Events() :
      events_(new Event::V())
    {
    }

    void Events::add(Event* event) {
      if (events_->size() == 0) {
	date_ = event->getWaveform().getTime().date();
      } else {
	if (date_ != event->getWaveform().getTime().date())
	  throw exception::Base("event date mismatch");
      }
      events_->push_back(event);
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
      if (events_->size() == 0) {
	date_ = date;
      } else {
	throw exception::Base("data::Events::setDate() setting of date not allowed");
      }
    }

    int Events::size() const {
      return events_->size();
    }

    void Events::sort() {
      events_->sort();
    }

    void Events::clear() {
      events_->clear();
    }

    Event::VI Events::erase(Event::VI start, Event::VI end) {
      return events_->erase(start, end);
    }

    const Event& Events::front() const {
      return events_->front();
    }

    Event& Events::front() {
      return events_->front();
    }

    const Event& Events::back() const {
      return events_->back();
    }

    Event& Events::back() {
      return events_->back();
    }

    Event::CVI Events::begin() const {
      return events_->begin();
    }

    Event::VI Events::begin() {
      return events_->begin();
    }

    Event::CVI Events::end() const {
      return events_->end();
    }

    Event::VI Events::end() {
      return events_->end();
    }

    std::string Events::appendToFile(const std::string& fileName, const unsigned short fileVersion) {
      if (events_->size() > 0) {

	EventsFile eventsFile(fileName);

	eventsFile.append(*this);
	
	return eventsFile.getFilename();
      }
      return "";
    }

    std::string Events::writeToFile(const std::string& fileName, const unsigned short fileVersion) {
      if (events_->size() > 0) {

	EventsFile eventsFile(fileName);

	eventsFile.write(*this);
	
	return eventsFile.getFilename();
      }
      return "";
    }

    void Events::readFromFile(const std::string& filename, const pt::time_duration& startTime, const pt::time_duration& endTime) {

      EventsFile eventsFile(filename);
      events_ = eventsFile.read(startTime, endTime);
      date_ = eventsFile.getHeader().getDate();
    }

  }
}
