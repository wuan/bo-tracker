#include <fstream>

#include "data/EventsHeader.h"
#include "data/Event.h"
#include "data/event/V1.h"
#include "data/event/V2.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    const char* EventsHeader::ID = "BOSF";

    const gr::date EventsHeader::STARTOFEPOCH = gr::date(1970, 1, 1);

    bool file_exists(std::string filename) {
      std::ifstream file;

      file.open(filename.c_str(), std::ios::in);
      file.close();

      return ! file.fail();
    }

    EventsHeader::EventsHeader(const gr::date& date, const unsigned short version) :
      date_(date),
      logger_("data.EventsHeader")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "construct() date " << date << " version " << version;
    }

    EventsHeader::~EventsHeader() {
    }

    const gr::date& EventsHeader::getDate() const {
      return date_;
    }

    void EventsHeader::setDate(const gr::date& date) {
      date_ = date;
    }

    unsigned int EventsHeader::getNumberOfEvents() const {
      return numberOfEvents_;
    }  

    unsigned int EventsHeader::getEventSize() const {
      return eventSize_;
    }  

    void EventsHeader::read(const std::string& filename) {
      
      if (! file_exists(filename)) {
	std::ostringstream oss;
	oss << "data::EventHeader::read() file '" << filename << "' does not exist";
	throw exception::Base(oss.str());
      }
      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::in | std::ios::binary);
      
      fstream.seekg(0, std::ios::beg);

      {
	char tempId[5];

	// check for correct file ID
	fstream.read(tempId, 4);
	
	std::string fileId(tempId);
	tempId[sizeof(tempId)-1] = 0;

	if (std::string(tempId) != ID) {
	  std::ostringstream oss;
	  oss << "data::EventsHeader bad file header '" << tempId << "' vs '" << ID << "'";
	  throw exception::Base(oss.str());
	}
      }

      {
	unsigned int fileEpochDays;

	// read file version from file
	fstream.read((char*)&fileEpochDays, sizeof(unsigned int));
	date_ = STARTOFEPOCH + gr::days(fileEpochDays);
      }

      // check if read position corresponds to header size
      assert(fstream.tellg() == getSize());

      // set object to create event objects 
      setFactory();

      data::Event::AP firstEvent = eventFactory_->createEvent(fstream, date_);
      eventSize_ = firstEvent->getSize();
      
      fstream.seekg(0, std::ios::end);
      unsigned int filesize = fstream.tellg();
      
      fstream.close();
      
      filesize -= getSize();

      numberOfEvents_ = filesize / eventSize_;

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() date " << date_ << " #events " << numberOfEvents_ << " eventsize " << eventSize_ << " filesize " << filesize;

      if (numberOfEvents_ * eventSize_ != filesize)
	throw exception::Base("data::EventsHeader file size mismatch");
    }

    Event::AP EventsHeader::createEvent(std::iostream& stream) const {
      return eventFactory_->createEvent(stream, date_);
    }
	
    std::string EventsHeader::formatFilename(const std::string& fileformat) const {

      if (! date_.is_not_a_date()) {

        gr::date_facet *datefacet = new gr::date_facet();
        datefacet->format(fileformat.c_str());

        std::ostringstream filenamestream;
        filenamestream.imbue(std::locale(std::locale::classic(), datefacet));
        filenamestream << date_;

        return filenamestream.str();
      } else {
	return fileformat;
	throw exception::Base("data::EventsHeader formatFilename() ERROR: no file date");
      }
    }

    void EventsHeader::write(const std::string& filename) const {

      if (date_ == gr::date(pt::not_a_date_time))
	throw exception::Base("data::EventsHeader writeHeader() invalid file date");

      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
      
      fstream.seekg(0, std::ios::beg);

      fstream.write(ID, 4);

      {
	unsigned int fileEpochDays = (date_ - STARTOFEPOCH).days();
	fstream.write((char*) &fileEpochDays, sizeof(unsigned int));
      }

      assert(fstream.tellg() == getSize());
      
      fstream.close();
    }

    void EventsHeader::setFactory() {

      switch (version_) {
	case 1:
	  eventFactory_ = EventFactory::AP(new event::V1Factory());
	  break;

	case 2:
	  eventFactory_ = EventFactory::AP(new event::V2Factory());
	  break;

	default:
	  std::ostringstream oss;
	  oss << "data::EventsHeader setFactory() unknown file version " << version_;
	  throw exception::Base(oss.str().c_str());
	  break;
      }
    }


    bool EventsHeader::fileExists(const std::string& filename) const {
      return file_exists(formatFilename(filename));
    }
    
    bool EventsHeader::operator==(const EventsHeader& other) {
      return date_ == other.date_ && version_ == other.version_;
    }
	
    bool EventsHeader::operator!=(const EventsHeader& other) {
      return !(*this == other);
    }

    unsigned int EventsHeader::getSize() const {
      return 10;
    }
    
  }
}

