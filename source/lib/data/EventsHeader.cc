#include <fstream>

#include "data/Format.h"
#include "data/EventsHeader.h"
#include "data/Event.h"
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

    EventsHeader::EventsHeader(const Format& dataFormat, const gr::date& date) :
      version_(1),
      date_(date),
      dataFormat_(dataFormat),
      numberOfEvents_(0),
      logger_("data.EventsHeader")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "construct() date " << date_ << ", format " << dataFormat_;
    }

    EventsHeader::~EventsHeader() = default;

    void EventsHeader::set(const Events& events) {
      date_ = events.getDate();
      dataFormat_ = events.getDataFormat();
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "set() date " << date_ << ", format " << dataFormat_;
    }

    const gr::date& EventsHeader::getDate() const {
      return date_;
    }

    const Format& EventsHeader::getDataFormat() const {
      return dataFormat_;
    }

    unsigned int EventsHeader::getNumberOfEvents() const {
      return numberOfEvents_;
    }  

    unsigned int EventsHeader::getEventSize() const {
      return Event::GetSize(dataFormat_);
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
	tempId[sizeof(tempId)-1] = 0;

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "read() ID: '" << tempId << "'";
	
	std::string fileId(tempId);

	if (std::string(tempId) != ID) {
	  std::ostringstream oss;
	  oss << "data::EventsHeader bad file header '" << tempId << "' vs '" << ID << "'";
	  throw exception::Base(oss.str());
	}
      }

      fstream.read((char*)&version_, sizeof(version_));
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() version: " << version_;

      {
	unsigned int fileEpochDays;

	// read file version from file
	fstream.read((char*)&fileEpochDays, sizeof(fileEpochDays));
	date_ = STARTOFEPOCH + gr::days(fileEpochDays);

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "read() epoch: " << fileEpochDays << " = " << date_;
      }

      dataFormat_ = Format(fstream);

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() data format: " << dataFormat_ << ", event size: " << getEventSize();

      // check if read position corresponds to header size
      assert(fstream.tellg() == getSize());

      fstream.seekg(0, std::ios::end);
      unsigned int filesize = fstream.tellg();
      
      fstream.close();
      
      filesize -= getSize();

      numberOfEvents_ = filesize / getEventSize();

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() numberofevents: " << filesize << " / " << getEventSize() << " = " << filesize / getEventSize() << " => filesize = " << numberOfEvents_ * getEventSize();

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "read() date " << date_ << " #events " << numberOfEvents_ << " eventsize " << getEventSize() << " filesize " << filesize;

      if (numberOfEvents_ * getEventSize() != filesize)
	throw exception::Base("data::EventsHeader file size mismatch");
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

      if (logger_.isDebugEnabled())
	logger_.debugStream() << "write() date " << date_ << " #events " << numberOfEvents_;

      if (date_ == gr::date(pt::not_a_date_time))
	throw exception::Base("data::EventsHeader write() invalid file date");

      std::fstream fstream;
      
      fstream.open(formatFilename(filename).c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

      if (!fstream.is_open()) {
	throw exception::Base("data::EventsHeader write() could not open file '" + formatFilename(filename) + "'");
      }
      
      fstream.seekg(0, std::ios::beg);

      fstream.write(ID, 4);

      fstream.write((char*)&version_, sizeof(version_));

      {
	unsigned int fileEpochDays = (date_ - STARTOFEPOCH).days();
	fstream.write((char*) &fileEpochDays, sizeof(fileEpochDays));
      }

      dataFormat_.toStream(fstream);

      assert(fstream.tellg() == getSize());
      
      fstream.close();
    }

    bool EventsHeader::fileExists(const std::string& filename) const {
      return file_exists(formatFilename(filename));
    }
    
    bool EventsHeader::operator==(const EventsHeader& other) {
      return date_ == other.date_ && dataFormat_ == other.dataFormat_;
    }
	
    bool EventsHeader::operator!=(const EventsHeader& other) {
      return !(*this == other);
    }

    Event::AP EventsHeader::createEvent(std::iostream& stream) const {
      return createEvent(stream, dataFormat_);
    }

    Event::AP EventsHeader::createEvent(std::iostream& stream, const Format& dataFormat) const {
      return Event::AP(new Event(dataFormat, date_, stream));
    }
	
    size_t EventsHeader::getSize() const {
      return 14;
    }
    
    std::ostream& operator<<(std::ostream& os, const EventsHeader& header) {
      os << "header(" << header.getDate() << ", " << header.getDataFormat() << ")";

      return os;
    }
  }
}

