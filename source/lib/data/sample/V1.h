#ifndef BLITZORTUNG_DATA_SAMPLE_VERSION1_H_
#define BLITZORTUNG_DATA_SAMPLE_VERSION1_H_

#include "namespaces.h"
#include "Logger.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class V1 for sample data version 1
      class V1 : public Base {
	
	public:
	  //! type definition for vector container
	  typedef boost::ptr_vector<V1> V;
	  typedef V::iterator VI;
	  typedef V::const_iterator CVI;
	  typedef boost::shared_ptr<V> VP;

	  //! constructor
	  V1();

	  //! destructor
	  virtual ~V1();

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
