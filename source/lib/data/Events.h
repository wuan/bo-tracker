#ifndef BLITZORTUNG_DATA_EVENTS_H_
#define BLITZORTUNG_DATA_EVENTS_H_

#include "namespaces.h"
#include "data/Event.h"
#include "data/EventsHeader.h"
#include "Logger.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    class EventsHeader;

    //! container class for events
    class Events {

      public:
        typedef Event::VI I;
        typedef Event::CVI CI;
	typedef std::shared_ptr<Events> P;
	typedef std::unique_ptr<Events> AP;

      private:
      
        //! date of events
	gr::date date_;

	//! data format
	Format dataFormat_;

	//! storage for events
	Event::V events_;

	//! index of first event in file (-1 if not set)
	int startIndex_;

	//! logger for this class
	mutable Logger logger_;

	//! add event reference by pointer to collection
	void add(Event*);

	//! replace events by events of given structure
	void replace(Events&);

	//! set events properties from or check events properties agains a single event
        void setOrCheckProperties(Event& event);

      public:

	//! create empty Events
	Events();
	
	//! create Events with given header information
	Events(const EventsHeader& header);

	//! create Events with date and dataformat
	Events(const gr::date& date, const Format& dataFormat);

	//! destructor
	virtual ~Events();

	Events(const Events&) = delete;
	Events& operator=(const Events&) = delete;

	//! add event to collection
	void add(Event::AP&&);

	//! add event to collection
	void add(Event::V::auto_type);

	//! add other event collection to event collection
	void add(Events&) throw(exception::Base);

	//! release event from events
	Event::AP release(Event::VI);

	//! set date of event collection
	void setDate(const gr::date& date);

	//! get date of event collection
	const gr::date& getDate() const;

	//! get dataFormat
	const Format& getDataFormat() const;

	//! set start index when events are read from a file
	void setStartIndex(int);

	//! get start index
	int getStartIndex() const;

	//! get number of events
	int size() const;

	//! sort events
	void sort();

	//! sort and filter event duplicates
	void unique();

	//! set event sort behaviour
        template< class Compare >
        void sort( Compare comp ) {
	  events_.sort(comp);
	}

	//! erase all events
	void clear();

	//! erase single event
	Event::VI erase(Event::VI);

	//! erase events between two iterators
	Event::VI erase(Event::VI, Event::VI);

	//! get const front of collection
	const Event& front() const;
	
	//! get front of collection
	Event& front();
	
	//! get const back of collection
	const Event& back() const;
	
	//! get back of collection
	Event& back();
	
	//! get const iterator to begin of collection
	Event::CVI begin() const;
	
	//! get iterator to begin of collection
	Event::VI begin();
	
	//! get const iterator to end of collection
	Event::CVI end() const;
	
	//! get iterator to end of collection
	Event::VI end();

	void transfer(Event::VI, Event::VI, Event::VI, Events&);

	void transfer(Event::VI, Events&);

	//
	// file operations
	//
	
	//! append current events to given file
	std::string appendToFile(const std::string&) const;

	//! write current events to given file
	std::string writeToFile(const std::string&);

	//! read events from given file
	void readFromFile(const std::string&, const pt::time_duration& starttime, const pt::time_duration& endtime = pt::time_duration(pt::not_a_date_time));

	//! read events from given file
	void readFromFile(const std::string&, const int startIndex=0, const int endIndex=-1);
    };

  }
}
#endif
