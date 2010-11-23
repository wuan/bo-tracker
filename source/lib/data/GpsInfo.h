#ifndef BLITZORTUNG_DATA_GPSINFO_H_
#define BLITZORTUNG_DATA_GPSINFO_H_

#include <boost/shared_ptr.hpp>

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

	//! time
	pt::ptime time_;

	//! longitude
	float longitude_;

	//! latitude
	float latitude_;

	//! altitude
	float altitude_;

	//! number of satellites
	unsigned char satelliteCount_;

	//! status
	char status_;

      public:

        //! create a waveform object
	/*!
	\param gps const reference to gps hardware
	*/
	GpsInfo(const hardware::gps::Base& gps);

	typedef std::auto_ptr<GpsInfo> AP;

	//! destroy a waveform object
	virtual ~GpsInfo();

	float getLongitude() const;
	float getLatitude() const;
	float getAltitude() const;
	unsigned char getSatelliteCount() const;
	char getStatus() const;
    };

  }
}
#endif
