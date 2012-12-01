#ifndef BLITZORTUNG_DATA_GPSINFO_H_
#define BLITZORTUNG_DATA_GPSINFO_H_

#include "namespaces.h"
#include "exception/Base.h"
#include "data/Gps.h"

namespace blitzortung {
  namespace data {

    //! class for gps information
    class GpsInfo {

      public:

	typedef std::unique_ptr<GpsInfo> AP;

      private:

	//! longitude
	float longitude_;

	//! latitude
	float latitude_;

	//! altitude
	short altitude_;

	//! number of satellites
	unsigned char satelliteCount_;

	//! status
	char status_;

      public:

        //! create a gpsinfo object
	/*!
	\param gps const reference to gps hardware
	*/
	GpsInfo(const data::Gps& gps);

        //! create a gpsinfo object from a stream
	/*!
	\param stream from which the object should be read
	*/
	GpsInfo(std::iostream& stream);

        //! create a gpsinfo object with given parameters
	/*!
	\param stream from which the object should be read
	*/
	GpsInfo(float longitude, float latitude, short altitude);
	
	//! destroy a waveform object
	virtual ~GpsInfo();

	GpsInfo(const GpsInfo&) = delete;
	GpsInfo& operator=(const GpsInfo&) = delete;

	//! returns longitude
	float getLongitude() const;

	//! returns latitude
	float getLatitude() const;

	//! returns altitude
	short getAltitude() const;

	//! returns the number of satellites available to the gps module
	unsigned short getSatelliteCount() const;

	//! returns the acquisition status of the gps module
	char getStatus() const;

	//! write to stream
	void toStream(std::iostream&);

	//! return size of structure in bytes	
	size_t getSize();

	//! static function to determine size of a particular waveform structure
	static size_t GetSize();

    };

    std::ostream& operator << (std::ostream& os, const GpsInfo&);

  }
}
#endif
