#ifndef BLITZORTUNG_DATA_SAMPLE_VERSION1_H_
#define BLITZORTUNG_DATA_SAMPLE_VERSION1_H_

#include "namespaces.h"
#include "data/sample/Base.h"

namespace blitzortung {
  namespace data {
    namespace sample {

      //! class for sample data version 1
      class V1 : public Base {
	
	public:
	  //! type definition for vector container
	  typedef boost::ptr_vector<V1> V;
	  typedef V::iterator VI;
	  typedef V::const_iterator CVI;
	  typedef boost::shared_ptr<V> VP;

	private:

	  pt::ptime time_;
	  float latitude_;
	  float longitude_;
	  short altitude_;
	  unsigned char gpsNumberOfSatellites_;
	  char gpsStatus_;
	  short peak1Offset_;
	  float peak1A_;
	  float peak1B_;

	  // maximal absolut value for channel A and B
	  static const int AD_MAX_VALUE;
	  // maximal absolute millivolt for channel A and B
	  static const int AD_MAX_VOLTAGE;
	  // absolute threshold millivolt for channel A and B
	  static const int AD_THRESHOLD_VOLTAGE;

	public:

	  //! constructor
	  V1();

	  //! destructor
	  virtual ~V1();

	  //! Creator
	  struct Creator : public Base::Creator {
	    virtual Base* operator()() const;
	  };
		  
	  //! getter for sample and peak time
	  virtual pt::ptime getTime(int index=0) const;

	  //! setter for sample and peak time
	  virtual void setTime(const pt::ptime&);

	  //! setter for peak time offset
	  virtual void setOffset(short offsetFactor, int index);

	  //! getter for x peak amplitude
	  virtual float getXAmplitude(int index) const;
	  
	  //! getter for y peak amplitude
	  virtual float getYAmplitude(int index) const;

	  //! setter for peak amplitude
	  virtual void setAmplitude(const float xamp, const float yamp, int index);

	  //! setter for antenna longitude value
	  virtual void setAntennaLongitude(const float longitude);

	  //! getter for antenna longitude value
	  virtual float getAntennaLongitude() const;

	  //! setter for antenna latitude value
	  virtual void setAntennaLatitude(const float latitude);

	  //! getter for antenna latitude value
	  virtual float getAntennaLatitude() const;

	  //! setter for antenna altitude value
	  virtual void setAntennaAltitude(const short altitude);

	  //! getter for antenna altitude value
	  virtual short getAntennaAltitude() const;

	  //! setter for gps satellite count
	  virtual void setGpsNumberOfSatellites(const unsigned char satelliteCount);

	  //! getter for gps satellite count
	  virtual unsigned char getGpsNumberOfSatellites() const;

	  //! setter for gps status
	  virtual void setGpsStatus(const char gpsStatus);

	  //! getter for gps status
	  virtual char getGpsStatus() const;

	  //! getter for format version
	  virtual unsigned short getVersion() const;

      };

    }
  }
}

#endif
