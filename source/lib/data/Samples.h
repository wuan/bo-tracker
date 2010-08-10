#ifndef BLITZORTUNG_DATA_SAMPLES_H_
#define BLITZORTUNG_DATA_SAMPLES_H_

#include "namespaces.h"
#include "data/sample/Base.h"
//#include "data/SamplesFile.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace data {

    //! class for containing samples
    class Samples : boost::noncopyable {

      protected:
	gr::date date_;

	sample::Base::VP entries_;

	const sample::Base::Creator& creator_;

	void init();

      public:

	Samples(const sample::Base::Creator& sampleCreator);

	//! add sample to collection
	void add(std::auto_ptr<sample::Base>);

	//! add other sample collection to sample collection
	void add(Samples&) throw(exception::Base);

	//! get date of sample collection
	const gr::date& getDate() const;

	//! set date of sample collection
	void setDate(const gr::date&);

	// collection iterator methods
	const sample::Base& front() const;
	sample::Base& front();
	const sample::Base& back() const;
	sample::Base& back();
	sample::Base::CVI begin() const;
	sample::Base::VI begin();
	sample::Base::CVI end() const;
	sample::Base::VI end();
    };

  }
}
#endif
