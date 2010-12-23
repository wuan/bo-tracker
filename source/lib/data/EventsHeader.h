#ifndef BLITZORTUNG_DATA_SAMPLESHEADER_H_
#define BLITZORTUNG_DATA_SAMPLESHEADER_H_

#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "Logger.h"
#include "data/Event.h"
#include "data/EventFactory.h"
#include "data/Format.h"
#include "data/event/V1Factory.h"
#include "data/event/V2Factory.h"

namespace blitzortung {
  namespace data {

    //! file io helper class for data::Events
    class EventsHeader {

      private:
	
	//! static constant for file id
	static const char *ID;

	//! static constant for date which marks the start of the epoch
	static const gr::date STARTOFEPOCH;

	//***********************************
	
	//! file date
	gr::date date_;

	//! events per entry
	unsigned short numberOfEvents_;

	//! data format
	data::Format dataFormat_;
	
	//***********************************

	//! event factory
	EventFactory::AP eventFactory_;

	//! class logger
	mutable Logger logger_;

      public:

	//! constructor
	EventsHeader(const gr::date& date = gr::date(gr::not_a_date_time), const unsigned short version = 0);

	//! destructor
	~EventsHeader();

	//! read header from stream
	void read(std::ifstream&);

	//! set header date
	void setDate(const gr::date&);

	//! get header date
	const gr::date& getDate() const;
	
	//! get number of events in file
	unsigned int getNumberOfEvents() const;

	//! get storage size of one event
	unsigned int getEventSize() const;

	//! set header version
	void setVersion(const unsigned short);

	//! get header version
	const unsigned short getVersion() const;

	bool operator==(const EventsHeader&);
	
	bool operator!=(const EventsHeader&);

	//! set creator object depending on file version number
	Event::AP createEvent(std::iostream& stream) const;

	std::string formatFilename(const std::string& fileformat) const;

	// check, if a file with given filename exists
	bool fileExists(const std::string&) const;

	//! read file header
	void read(const std::string&);
	
	//! write file header
	void write(const std::string&) const;
	
	unsigned int getSize() const;
    };

  }
}

#endif

