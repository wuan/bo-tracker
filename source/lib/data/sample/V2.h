#ifndef BLITZORTUNG_DATA_SAMPLE_VERSION2_H_
#define BLITZORTUNG_DATA_SAMPLE_VERSION2_H_

#include "namespaces.h"
#include "Logger.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class V2 for sample data version 2
      class V2 : public Base {
	
	public:
	  //! type definition for vector container
	  typedef boost::ptr_vector<V2> V;
	  typedef V::iterator VI;
	  typedef V::const_iterator CVI;
	  typedef boost::shared_ptr<V> VP;

	  //! constructor
	  V2();

	  //! destructor
	  virtual ~V2();

	  //! Creator
	  struct Creator : public Base::Creator {
	    virtual Base* operator()() const;
	  };

	  //! getter for format version
	  virtual unsigned short getVersion() const;

	  //! getter for number of samples in waveform
	  unsigned short getNumberOfSamples() const;
      };

    }
  }
}

#endif
