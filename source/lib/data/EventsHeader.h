#ifndef BLITZORTUNG_DATA_SAMPLESHEADER_H_
#define BLITZORTUNG_DATA_SAMPLESHEADER_H_

#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "Logger.h"
#include "data/Event.h"
#include "data/Events.h"
#include "data/Format.h"

namespace blitzortung {
  namespace data {

    class Events;

    //! file io helper class for data::Events
    class EventsHeader {

      private:
	
	//! static constant for file id
	static const char *ID;

	//! static constant for date which marks the start of the epoch
	static const gr::date STARTOFEPOCH;

	//! file format version
	unsigned short version_;

	//! file date
	gr::date date_;

	//! data format
	Format dataFormat_;
	
	//! events per entry
	unsigned int numberOfEvents_;

	//***********************************

	//! class logger
	mutable Logger logger_;

      public:

	//! constructor
	EventsHeader(const Format& dataFormat=Format(0,0,0), const gr::date& date = gr::date(gr::not_a_date_time));

	//! destructor
	virtual ~EventsHeader();

	//! read header from stream
	void read(std::ifstream&);

	//! set header date
	void set(const Events&);

	//! get header date
	const gr::date& getDate() const;

	//! get data format
	const Format& getDataFormat() const;
	
	//! get number of events in file
	unsigned int getNumberOfEvents() const;

	//! get storage size of one event
	unsigned int getEventSize() const;

	bool operator==(const EventsHeader&);
	
	bool operator!=(const EventsHeader&);

	std::string formatFilename(const std::string& fileformat) const;

	// check, if a file with given filename exists
	bool fileExists(const std::string&) const;

	//! read file header
	void read(const std::string&);
	
	//! write file header
	void write(const std::string&) const;
	
	//! create event from stream
	Event::AP createEvent(std::iostream& stream) const;

	//! create event from stream with given format
	Event::AP createEvent(std::iostream& stream, const Format& dataFormat) const;

	//! return size of header in bytes
	size_t getSize() const;
    };

    //! stream output operator
    std::ostream& operator<<(std::ostream& os, const EventsHeader& header);

  }
}

#endif

