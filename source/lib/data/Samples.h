#ifndef BLITZORTUNG_DATA_SAMPLES_H_
#define BLITZORTUNG_DATA_SAMPLES_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/sample/Base.h"
#include "data/SamplesFileHeader.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for containing samples
    class Samples : boost::noncopyable {

      public:
	typedef boost::shared_ptr<Samples> P;
	typedef sample::Base Sample;

      protected:
	SamplesFileHeader header_;

	Sample::VP samples_;

	void init();

	void add(sample::Base*);

      public:

	Samples();

	//! add sample to collection
	void add(sample::Base::AP);

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

        template< class Compare >
        void sort( Compare comp ) {
	  samples_->sort(comp);
	}

	//! erase all samples
	void clear();

	//! erase samples between two iterators
	Sample::VI erase(Sample::VI, Sample::VI);

	// collection iterator methods
	const sample::Base& front() const;
	sample::Base& front();
	const sample::Base& back() const;
	sample::Base& back();
	Sample::CVI begin() const;
	Sample::VI begin();
	Sample::CVI end() const;
	Sample::VI end();

	// file operations
	
	//! append current samples to given file
	void appendToFile(const std::string&, const unsigned short fileVersion = 0);

	//! write current samples to given file
	void writeToFile(const std::string&);

	//! read samples from given file
	void readFromFile(const std::string&);
    };

  }
}
#endif
