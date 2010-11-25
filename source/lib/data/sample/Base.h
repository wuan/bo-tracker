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

	  
	  //! sample waveform
	  Sample::Waveform::AP waveform_;

	  //! sample gpsInfo
	  GpsInfo::AP gpsInfo_;

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

	  //! setter for sample
	  void set(data::Sample::AP);

	  //! getter for sample waveform
	  virtual const data::Sample::Waveform& getWaveform() const;

	  //! getter for sample gpsInfo
	  virtual const data::GpsInfo& getGpsInfo() const;

	  //! getter for sample waveform timestamp
	  virtual const pt::ptime& getTime() const;

	  //! getter for sample waveform timedelta
	  virtual const pt::time_duration& getTimeDelta() const;

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
