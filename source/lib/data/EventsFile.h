#ifndef BLITZORTUNG_DATA_SAMPLESFILE_H_
#define BLITZORTUNG_DATA_SAMPLESFILE_H_

#include <iostream>
#include <fstream>
#include <iomanip>

#include "namespaces.h"
#include "Logger.h"
#include "data/Events.h"
#include "data/EventsHeader.h"
#include "data/Event.h"

namespace blitzortung {
  namespace data {

    //! file io helper class for data::Events
    class EventsFile {

      private:
	
	//! file stream object
	std::fstream fstream_;

	//! file stream openmode
	std::ios_base::openmode openmode_;

	//! file name
	std::string name_;

	//! header information
	EventsHeader header_;

	//! logger for class
	mutable Logger logger_;

	//! read a single event
	Event::AP readEvent();

	//! append/write events to file with given filename
	void writeEvents(const Events&, bool append=false);

	//! open the fstream
	void open(std::ios_base::openmode);

	//! check if fstream is open
	bool isOpen() const;

	//! close the fstream
	void close();

        //! do btree search for event position
	unsigned int findEvent(const pt::time_duration& target, unsigned int start, unsigned int end);

	//! set read position to element with given index
	void seekEvent(int index);

	//! read index range
	Events::AP readInternal(int index, int size);

      public:

	//! constructor
	EventsFile(const std::string&);

	//! destructor
	~EventsFile();

	//! set Filename
	void setFilename(const std::string&);

	//! get Filename
	const std::string getFilename() const;

	//! get Header
	const EventsHeader& getHeader() const;

	//! append or create a new file
	void append(const Events&);
	
	//! overwrite or create a new file
	void write(const Events&);
	
	//! read index range
	Events::AP read(const int index, int size=-1);

	//! read time range
	Events::AP read(const pt::time_duration& start, const pt::time_duration& end = pt::time_duration(pt::not_a_date_time));
    };

  }
}

#endif

