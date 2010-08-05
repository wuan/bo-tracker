#ifndef BLITZORTUNG_DATA_SAMPLE_VERSION1_H_
#define BLITZORTUNG_DATA_SAMPLE_VERSION1_H_

#include "namespaces.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class for sample data version 1
      class Version1 : public Base {
	
	public:
	  //! type definition for vector container
	  typedef boost::ptr_vector<Version1> V;
	  typedef V::iterator VI;
	  typedef V::const_iterator CVI;
	  typedef boost::shared_ptr<V> VP;

          // time since 00:00 in ns
	  typedef unsigned long long int TYPE_TIME;
	  // factor time offset of peak in 3150 ns
	  typedef unsigned short TYPE_OFFSET;
	  // station coordinates
	  typedef float TYPE_COORDINATE;
	  // amplitude values at peak
	  typedef signed char TYPE_AMPLITUDE;
	  // simple count type
	  typedef unsigned char TYPE_GPSNUMBEROFSATELLITES;
	  // simple char type
	  typedef char TYPE_GPSSTATUS;

	  static const unsigned int IDX_TIME;
	  static const unsigned int IDX_LONGITUDE;
	  static const unsigned int IDX_LATITUDE;
	  static const unsigned int IDX_GPSNUMBEROFSATELLITES;
	  static const unsigned int IDX_GPSSTATUS;
	  static const unsigned int IDX_OFFSET1;
	  static const unsigned int IDX_XAMP1;
	  static const unsigned int IDX_YAMP1;
	  static const unsigned int DATASIZE;

	public:

	  //! constructor
	  Version1();

	  //! destructor
	  virtual ~Version1();

	  //! Creator
	  struct Creator : public Base::Creator {
	    virtual Base* operator()() const;
	  };
		  
	  //! getter for sample and peak time
	  virtual pt::time_duration getTime(int index=0) const;

	  //! setter for sample and peak time
	  virtual void setTime(const pt::time_duration&);

	  //! setter for peak time offset
	  virtual void setOffset(unsigned short nanoseconds, int index);

	  //! getter for x peak amplitude
	  virtual signed short getXAmplitude(int index) const;
	  
	  //! getter for y peak amplitude
	  virtual signed short getYAmplitude(int index) const;

	  //! setter for peak amplitude
	  virtual void setAmplitude(const signed short xamp, const signed short yamp, int index);

	  //! setter for antenna longitude value
	  virtual void setAntennaLongitude(const float longitude);

	  //! getter for antenna longitude value
	  virtual float getAntennaLongitude() const;

	  //! setter for antenna latitude value
	  virtual void setAntennaLatitude(const float latitude);

	  //! getter for antenna latitude value
	  virtual float getAntennaLatitude() const;

	  //! setter for gps satellite count
	  virtual void setGpsNumberOfSatellites(const unsigned char satelliteCount);

	  //! getter for gps satellite count
	  virtual unsigned char getGpsNumberOfSatellites() const;

	  //! setter for gps status
	  virtual void setGpsStatus(const char gpsStatus);

	  //! getter for gps status
	  virtual char getGpsStatus() const;

	  //! return size of current data format
	  virtual unsigned int getDataSize() const;
      };

    }
  }
}

#endif
