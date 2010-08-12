#ifndef BLITZORTUNG_DATA_SAMPLES_H_
#define BLITZORTUNG_DATA_SAMPLES_H_

#include <boost/shared_ptr.hpp>

#include "namespaces.h"
#include "data/sample/Base.h"
//#include "data/SamplesFile.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for containing samples
    class Samples : boost::noncopyable {

      public:
	typedef boost::shared_ptr<Samples> P;
	typedef sample::Base Sample;

      protected:
	gr::date date_;

	Sample::VP samples_;

	const Sample::Creator& creator_;

	void init();

      public:

	Samples(const Sample::Creator& sampleCreator);

	//! add sample to collection
	void add(std::auto_ptr<sample::Base>);

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

	//! clear or erase samples
	void clear();

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
    };

  }
}
#endif
