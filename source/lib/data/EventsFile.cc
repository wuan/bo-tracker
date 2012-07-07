#include "data/Format.h"
#include "data/EventsFile.h"
#include "data/Event.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    EventsFile::EventsFile(const std::string& name) :
      fstream_(),
      openmode_(),
      name_(name),
      header_(),
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

    void EventsFile::seekEvent(int index) {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "seekEvent() " << index;
      fstream_.seekg(header_.getSize() + index * header_.getEventSize(), std::ios::beg);
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
      header_.set(events);
      writeEvents(events); 
    }

    void EventsFile::append(const Events& events) {
      header_.set(events);

      if (header_.fileExists(name_)) {
	writeEvents(events, true);
      } else {
	writeEvents(events);
      }
    }
    
    void EventsFile::writeEvents(const Events& events, bool append) {

      if (events.size() > 0) {

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "writeEvents() " << name_ << " append: " << append;

	if (events.getDataFormat() != events.front().getWaveform().getFormat()) {
	  std::cout << std::endl << "  " << events.getDataFormat() << " vs. " << events.front().getWaveform().getFormat() << std::endl;
	  throw exception::Base("data::EventsFile() writeEvents() events format mismatch");
	}

	setFilename(name_);

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
	    logger_.debugStream() << "writeEvents() write header " << header_;
	}
	
	open(std::ios::out | std::ios::binary | std::ios::app);

	if (logger_.isDebugEnabled()) {
	  logger_.debugStream() << "writeEvents() write " << events.size() << " events (" << events.front().getStorageSize() << " bytes per event)";
	  logger_.debugStream() << "writeEvents() wfm size " << events.getDataFormat() << " " << events.front().getWaveform().getStorageSize();
	  logger_.debugStream() << "writeEvents() format " << events.getDataFormat() << " " << &events.getDataFormat();
	}

        fstream_.seekg(0, std::ios::end);
	logger_.debugStream() << "writeEvents()   position " << fstream_.tellg();
	unsigned int position = fstream_.tellg();

	for (Event::CVI event = events.begin(); event != events.end(); event++) {
	  event->toStream(fstream_);
	  if (logger_.isDebugEnabled()) {
	    logger_.debugStream() << "writeEvents()   position " << position << " delta " << (int(fstream_.tellg()) - position);
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
      seekEvent(middle);
      Event::AP event = header_.createEvent(fstream_);

      // recursive btree search
      if (event->getWaveform().getTimestamp().time_of_day() > target) {
	// lower interval
	end = middle;
      } else {
	// upper interval
	start = middle;
      }

      // check, if search has reached last iteration
      if ((end - start) <= 1) {

	seekEvent(start);

	event = header_.createEvent(fstream_);

	// return index position
	if (event->getWaveform().getTimestamp().time_of_day() >= target) {
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
	logger_.debugStream() << "read(" << start << ", " << end << ")";
     
      header_.read(name_);

      open(std::ios::in | std::ios::binary);

      unsigned int startIndex = 0;
      unsigned int endIndex = header_.getNumberOfEvents();

      if (! start.is_not_a_date_time())
	startIndex = findEvent(start, 0, endIndex);

      if (! end.is_not_a_date_time())
	endIndex = findEvent(end, startIndex, endIndex);

      unsigned int numberOfEvents = endIndex - startIndex;

      Events::AP events(readInternal(startIndex, numberOfEvents));

      close();

      return events;
    }

    Events::AP EventsFile::read(int startIndex, int numberOfEvents) {

      header_.read(name_);

      if (startIndex < 0)
	startIndex = header_.getNumberOfEvents() + startIndex;
      if (numberOfEvents < 0)
	numberOfEvents = header_.getNumberOfEvents() - startIndex + numberOfEvents + 1;

      open(std::ios::in | std::ios::binary);

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() read " << numberOfEvents << " events (" << startIndex << " - " << startIndex + numberOfEvents - 1 << ")";

      Events::AP events(readInternal(startIndex, numberOfEvents));

      close();

      return events;
    }

    Events::AP EventsFile::readInternal(int startIndex, int numberOfEvents) {

      Events::AP events(new Events(header_));
      
      seekEvent(startIndex);

      events->setStartIndex(startIndex);

      for (unsigned int i=0; i < (unsigned int)(numberOfEvents); i++) {
	events->add(header_.createEvent(fstream_));
      }

      return events;
    }
  }
}
