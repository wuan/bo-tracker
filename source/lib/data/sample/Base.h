#ifndef BLITZORTUNG_DATA_SAMPLE_BASE_H_
#define BLITZORTUNG_DATA_SAMPLE_BASE_H_

#include <iostream>
#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "namespaces.h"
#include "exception/Base.h"
#include "data/Sample.h"
#include "util/Stream.h"

namespace blitzortung {
  namespace data {
    namespace sample {
	      
      //! Base class for Samples
      class Base : boost::noncopyable {

	public:

	  //! type definition for vector container
	  typedef boost::ptr_vector<Base> V;
	  typedef V::iterator VI;
	  typedef V::const_iterator CVI;
	  typedef boost::shared_ptr<V> VP;
	  typedef std::auto_ptr<Base> AP;

	protected:

	  //! class Size to determine sizes of binary structures
	  class Size {
	    private:
	      unsigned int size_;

	    public:
	      Size() :
		size_(0)
	    {
	    }

	      template<typename T>
		unsigned int add(const T& value) {
		  size_ += sizeof(value);
		  return size_;
		}
	      unsigned int get() const {
		return size_;
	      }
	  };
	  
	  //! sample waveform
	  Sample::Waveform::AP waveform_;

	  //! gps latitude
	  float latitude_;

	  //! gps longitude
	  float longitude_;

	  //! gps altitude
	  short altitude_;

	  //! number of sattelites used by gps
	  unsigned char gpsNumberOfSatellites_;

	  //! gps status character
	  char gpsStatus_;
	  
	public:

	  //! constructor
	  Base();

	  //! Destructor
	  virtual ~Base();

	  //! Creator
	  struct Creator {

	    typedef boost::shared_ptr<Creator> P;
	    typedef std::auto_ptr<Creator> AP;

	    virtual Base* operator()() const {
	      throw exception::Base("data::sample::Base::Creator() being used!");
	    }
	  };

	  //! setter for sample waveform
	  void set(data::Sample::AP);

	  //! getter for sample waveform
	  virtual const data::Sample::Waveform& getWaveform() const;

	  //! getter for sample waveform timestamp
	  virtual const pt::ptime& getTime() const;

	  //! getter for antenna longitude value
	  virtual float getAntennaLongitude() const;

	  //! getter for antenna latitude value
	  virtual float getAntennaLatitude() const;

	  //! getter for antenna altitude value
	  virtual short getAntennaAltitude() const;

	  //! getter for gps satelllite count
	  virtual unsigned char getGpsNumberOfSatellites() const;

	  //! getter for gps status
	  virtual char getGpsStatus() const;

	  //! getter for sample format version
	  virtual unsigned short getVersion() const = 0;
	  
	  //! getter for amount of samples in waveform
	  virtual unsigned short getNumberOfSamples() const = 0;

	  //! write binary object data to stream
	  virtual void toStream(std::iostream&) const;
	  
	  //! read binary object data from stream
	  virtual void fromStream(std::iostream&, const gr::date&);
	  
	  //! get binary storage size of sample
	  virtual unsigned int getSize() const;

	  //! comparison operator <
	  bool operator<(const Base &) const;

	  //! class to compare timestamp of Strokes
	  struct CompareAmplitude : public std::binary_function<Base, Base, bool> {
	    bool operator()(const first_argument_type& x, const second_argument_type& y) const;
	  };

      };
      
      std::ostream& operator << (std::ostream& os, const Base&);
    }
  }
}

#endif
