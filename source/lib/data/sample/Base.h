#ifndef BLITZORTUNG_DATA_SAMPLE_BASE_H_
#define BLITZORTUNG_DATA_SAMPLE_BASE_H_

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "namespaces.h"

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

	public:

	  //! constructor
	  Base();

	  //! Destructor
	  virtual ~Base();

	  //! Creator
	  struct Creator {
	    virtual Base* operator()() const = 0;
	  };

	  //! getter for sample and peak time
	  virtual pt::time_duration getTime(int index=0) const = 0;

	  //! setter for sample and peak time
	  virtual void setTime(const pt::time_duration&) = 0;

	  //! setter for peak time by the offset to the sample time
	  virtual void setOffset(short offsetfactor, int index) = 0;

	  //! getter for peak amplitude
	  virtual float getXAmplitude(int index) const = 0;

	  //! getter for peak amplitude
	  virtual float getYAmplitude(int index) const = 0;

	  //! getter for peak amplitude
	  virtual float getAmplitude(int index) const;

	  //! setter for peak amplitude
	  virtual void setAmplitude(const float xamp, const float yamp, int index) = 0;

	  //! setter for antenna longitude value
	  virtual void setAntennaLongitude(const float longitude) = 0;

	  //! getter for antenna longitude value
	  virtual float getAntennaLongitude() const = 0;

	  //! setter for antenna latitude value
	  virtual void setAntennaLatitude(const float latitude) = 0;

	  //! getter for antenna latitude value
	  virtual float getAntennaLatitude() const = 0;

	  //! setter for antenna altitude value
	  virtual void setAntennaAltitude(const short altitude) = 0;

	  //! getter for antenna altitude value
	  virtual short getAntennaAltitude() const = 0;

	  //! setter for gps satellite count
	  virtual void setGpsNumberOfSatellites(const unsigned char satelliteCount) = 0;

	  //! getter for gps satelllite count
	  virtual unsigned char getGpsNumberOfSatellites() const = 0;

	  //! setter for gps status
	  virtual void setGpsStatus(const char gpsStatus) = 0;

	  //! getter for gps status
	  virtual char getGpsStatus() const = 0;

	  //! comparison operator <
	  bool operator<(const Base &) const;

	  //! class to compare timestamp of Strokes
	  struct CompareAmplitude : public std::binary_function<Base, Base, bool> {
	    bool operator()(const first_argument_type& x, const                   second_argument_type& y) const;
	  };

      };
      
      std::ostream& operator << (std::ostream& os, const Base&);
    }
  }
}

#endif
