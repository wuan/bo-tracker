
#include "data/EventsFile.h"
#include "data/Event.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    EventsFile::EventsFile(const std::string& name) :
      name_(name),
      logger_("data.EventsFile")
    {
    }

    bool EventsFile::isOpen() const {
      return fstream_.is_open();
    }

    void EventsFile::setFilename(const std::string& name) {
      name_ = name;
    }

    const std::string EventsFile::getFilename() const {
      return header_.formatFilename(name_);
    }

    void EventsFile::open(std::ios_base::openmode openmode) {

      bool openFile = false;

      if (isOpen()) {
	if (openmode != openmode_) {
	  close();
	  openFile = true;
	}
      } else {
	openFile = true;
      }

      const std::string filename = header_.formatFilename(name_);
      if (openFile) {
	fstream_.open(filename.c_str(), openmode);
	openmode_ = openmode;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "open() open file '" << filename << "' with mode " << openmode_;
      }
    }

    void EventsFile::close() {
      if (isOpen()) {
	fstream_.close();
      }
    }

    EventsFile::~EventsFile() {
    }

    const EventsHeader& EventsFile::getHeader() const {
      return header_;
    }

    void EventsFile::write(const Events& events) {
      writeEvents(name_, events); 
    }

    void EventsFile::append(const Events& events) {

      header_.set(events);

      if (header_.fileExists(name_)) {
	writeEvents(name_, events, true);
      } else {
	writeEvents(name_, events);
      }
    }
    
    void EventsFile::writeEvents(const std::string& name, const Events& events, bool append) {

      if (events.size() > 0) {

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "writeEvents() " << name << " append: " << append;

	header_.set(events);

	setFilename(name);

	if (append) {
	  EventsHeader header;
	  header.read(getFilename());

	  if (header != header_)
	    throw exception::Base("data::EventsFile() writeEvents() header mismatch during append");

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "writeEvents() append";
	} else {
	  header_.write(getFilename());
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "writeEvents() write header " << header_.getDate() << " " << header_.getDataFormat() << " ";
	}
	
	open(std::ios::out | std::ios::binary | std::ios::app);

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "writeEvents() write " << events.size() << " events (" << events.front().getSize() << " bytes per event)";

	unsigned int position = fstream_.tellg();

	for (Event::CVI event = events.begin(); event != events.end(); event++) {
	  event->toStream(fstream_);
	  if (logger_.isDebugEnabled()) {
	    logger_.debugStream() << "writeEvents()   position " << fstream_.tellg() << " delta " << (int(fstream_.tellg()) - position);
	    position = fstream_.tellg();
	  }

	}

	close();
      }
    }

    unsigned int EventsFile::findEvent(const pt::time_duration& target, unsigned int start, unsigned int end) {
      // determine middle of interval for next btree search iteration
      unsigned int middle = (start + end) / 2;

      // read event
      fstream_.seekg(header_.getSize() + middle * header_.getEventSize(), std::ios::beg);
      Event::AP event = header_.createEvent(fstream_);

      // recursive btree search
      if (event->getWaveform().getTime().time_of_day() > target) {
	// lower interval
	end = middle;
      } else {
	// upper interval
	start = middle;
      }

      // check, if search has reached last iteration
      if ((end - start) <= 1) {

	fstream_.seekg(header_.getSize() + start * header_.getEventSize(), std::ios::beg);

	event = header_.createEvent(fstream_);

	// return index position
	if (event->getWaveform().getTime().time_of_day() >= target) {
	  return start;
	} else {
	  return end;
	}
      } else {
	// do next iteration
	return findEvent(target, start, end);
      }
    }
		
    Events::AP EventsFile::read(const pt::time_duration& start, const pt::time_duration& end) {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read()";

      header_.read(name_);
     
      open(std::ios::in | std::ios::binary);

      unsigned int startIndex = 0;
      unsigned int endIndex = header_.getNumberOfEvents();

      if (! start.is_not_a_date_time())
	startIndex = findEvent(start, 0, endIndex);

      if (! end.is_not_a_date_time())
	endIndex = findEvent(end, startIndex, endIndex);

      fstream_.seekg(header_.getSize() + startIndex * header_.getEventSize() , std::ios::beg);

      unsigned int numberOfEvents = endIndex - startIndex;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() read " << numberOfEvents << " events (" << startIndex << " - " << endIndex - 1 << ")";

      Events::AP events(new Events(header_));
      
      for(unsigned int i=0; i < numberOfEvents; i++) {
	Event::AP event(header_.createEvent(fstream_));

	events->add(event);
      }

      close();
      
      return events;
    }
  }
}
