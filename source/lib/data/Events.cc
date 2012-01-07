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

    Events::Events(const gr::date& date, const Format& dataFormat) :
      date_(date),
      logger_("data.Events")
    {
      dataFormat_ = dataFormat;
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create(" << date << ", " << dataFormat << ") " << this;
    }

    Events::Events(const EventsHeader& header) :
      date_(header.getDate())
    {
      dataFormat_ = header.getDataFormat();
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "create(header) " << this << " " << header;
    }

    Events::~Events() {
      if (logger_.isDebugEnabled())
	logger_.debug("destroy");
    }

    void Events::add(Event* event) {
      setOrCheckProperties(*event);
      events_.push_back(event);
    }

    void Events::add(Event::AP&& event) {
      add(event.release());
    }

    void Events::add(Event::V::auto_type event) {
      add(event.release());
    }

    void Events::add(Events& source) throw(exception::Base) {
      transfer(end(), source);
    }

    void Events::setOrCheckProperties(Event& event) {
      const gr::date& date = event.getWaveform().getTime().date();
      Format dataFormat;
      if (!event.getWaveform().isEmpty()) {
	dataFormat = event.getWaveform().getArray().getFormat();
      }

      if (size() == 0) {
        date_ = date;
	dataFormat_ = dataFormat;
      } else {
	if (date_ != date) 
	  throw exception::Base("data::Events::setOrCheckProperties() date mismatch");
	if (dataFormat_ != dataFormat)
	  throw exception::Base("data::Events::setOrCheckProperties() date mismatch");
      }

      if (!event.getWaveform().isEmpty()) {
        event.updateFormatRef(dataFormat_);
      }
    }

    const gr::date& Events::getDate() const {
      return date_;
    }

    const Format& Events::getDataFormat() const {
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
      for (auto event=begin; event != end; event++)
        setOrCheckProperties(*event);

      events_.transfer(target, begin, end, source.events_);
    }

    void Events::transfer(Event::VI target, Events& source) {
      transfer(target, source.begin(), source.end(), source);
    }

    Event::AP Events::release(Event::VI target) {
      return Event::AP(events_.release(target).release());
    }

    std::string Events::appendToFile(const std::string& fileName) const {
      if (size() > 0) {

	if (logger_.isDebugEnabled()) {
	  logger_.debugStream() << "appendToFile() " << fileName << " (" << size() << " events)";
	}

	EventsFile eventsFile(fileName);
	eventsFile.append(*this);
	return eventsFile.getFilename();
      }
      return "";
    }

    std::string Events::writeToFile(const std::string& fileName) {
      if (size() > 0) {

	if (logger_.isDebugEnabled()) {
	  logger_.debugStream() << "writeToFile() " << fileName << " (" << size() << " events)";
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

    void Events::readFromFile(const std::string& fileName, const int startIndex, const int endIndex) {

      if (logger_.isDebugEnabled()) {
	logger_.debugStream() << "readFromFile() " << fileName << " (from " << startIndex << " to " << endIndex <<")";
      }

      EventsFile eventsFile(fileName);
      replace(*(eventsFile.read(startIndex, endIndex)));
    }

    void Events::replace(Events& source) {

      date_ = source.date_;
      dataFormat_ = source.dataFormat_;

      clear();
      transfer(end(), source);
    }


  }
}
