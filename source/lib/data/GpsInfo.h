#ifndef BLITZORTUNG_DATA_GPSINFO_H_
#define BLITZORTUNG_DATA_GPSINFO_H_

#include "namespaces.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      class Base;
    }
  }

  namespace data {

    //! class for gps information
    class GpsInfo : boost::noncopyable {

      private:

	//! longitude
	float longitude_;

	//! latitude
	float latitude_;

	//! altitude
	short altitude_;

	//! number of satellites
	unsigned char numberOfSatellites_;

	//! status
	char status_;

      public:

        //! create a gpsinfo object
	/*!
	\param gps const reference to gps hardware
	*/
	GpsInfo(const hardware::gps::Base& gps);

        //! create a gpsinfo object from a stream
	/*!
	\param stream from which the object should be read
	*/
	GpsInfo(std::iostream& stream);
	
	GpsInfo();

	typedef std::unique_ptr<GpsInfo> AP;

	//! destroy a waveform object
	virtual ~GpsInfo();

	//! returns longitude
	float getLongitude() const;

	//! returns latitude
	float getLatitude() const;

	//! returns altitude
	short getAltitude() const;

	//! returns the number of satellites available to the gps module
	unsigned short getNumberOfSatellites() const;

	//! returns the acquisition status of the gps module
	char getStatus() const;

	//! write to stream
	void toStream(std::iostream&);

	//! return size of structure in bytes	
	unsigned int getSize();

	//! static function to determine size of a particular waveform structure
	static unsigned int GetSize();

    };

    std::ostream& operator << (std::ostream& os, const GpsInfo&);

  }
}
#endif
