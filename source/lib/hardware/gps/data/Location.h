#ifndef BLITZORTUNG_HARDWARE_GPS_DATA_LOCATION_H_
#define BLITZORTUNG_HARDWARE_GPS_DATA_LOCATION_H_

#include "namespaces.h"
#include "util/RingBuffer.h"
#include "hardware/gps/data/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {
      namespace data {

	class Location : public Base {
	  private:

	    util::RingBuffer<double> longitude_;
	    util::RingBuffer<double> latitude_;
	    util::RingBuffer<double> altitude_;

	    static double getCoordFromNmea(const std::string &);

	  public:
	    Location();
	    virtual ~Location();

	    void addLongitude(const std::string &, const char);
	    double getLongitude() const;

	    void addLatitude(const std::string &, const char);
	    double getLatitude() const;

	    void addAltitude(const std::string &);
	    double getAltitude() const;

	};

      }
    }
  }
}

#endif
