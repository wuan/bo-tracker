#ifndef BLITZORTUNG_DATA_SAMPLES_H_
#define BLITZORTUNG_DATA_SAMPLES_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/sample/Base.h"
#include "data/SamplesHeader.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for containing samples
    class Samples : boost::noncopyable {

      public:
        typedef sample::Base::VI I;
        typedef sample::Base::CVI CI;
	typedef boost::shared_ptr<Samples> P;
	typedef sample::Base Sample;

      protected:
        //! header for samples
	SamplesHeader header_;

	//! storage for samples
	Sample::VP samples_;

	//! add sample reference by pointer to collection
	void add(sample::Base*);

      public:

	Samples();

	//! add sample to collection
	void add(sample::Base::AP);

	//! add sample to collection
	void add(sample::Base::V::auto_type);

	//! add other sample collection to sample collection
	void add(Samples&) throw(exception::Base);

	//! get date of sample collection
	const gr::date& getDate() const;

	//! set date of sample collection
	void setDate(const gr::date&);

	//! get number of samples
	int size() const;

	//! sort samples
	void sort();

	//! set sample sort behaviour
        template< class Compare >
        void sort( Compare comp ) {
	  samples_->sort(comp);
	}

	//! erase all samples
	void clear();

	//! erase samples between two iterators
	Sample::VI erase(Sample::VI, Sample::VI);

	//! get const front of collection
	const sample::Base& front() const;
	
	//! get front of collection
	sample::Base& front();
	
	//! get const back of collection
	const sample::Base& back() const;
	
	//! get back of collection
	sample::Base& back();
	
	//! get const iterator to begin of collection
	Sample::CVI begin() const;
	
	//! get iterator to begin of collection
	Sample::VI begin();
	
	//! get const iterator to end of collection
	Sample::CVI end() const;
	
	//! get iterator to end of collection
	Sample::VI end();

	//
	// file operations
	//
	
	//! append current samples to given file
	std::string appendToFile(const std::string&, const unsigned short fileVersion = 0);

	//! write current samples to given file
	std::string writeToFile(const std::string&, const unsigned short fileVersion = 0);

	//! read samples from given file
	void readFromFile(const std::string&, const pt::time_duration& starttime = pt::time_duration(pt::not_a_date_time), const pt::time_duration& endtime = pt::time_duration(pt::not_a_date_time));
    };

  }
}
#endif
