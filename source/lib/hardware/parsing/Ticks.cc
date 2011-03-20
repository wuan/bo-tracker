#include "hardware/parsing/Ticks.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      Ticks::Ticks(const std::vector<std::string> fields) :
	Base(),
	logger_("hardware.parsing.Ticks()")
      {
	unsigned int counterIndex = 0;
	unsigned int gpsStatusIndex = 0;
	unsigned int timeIndex = 0;
	unsigned int dateIndex = 0;
	unsigned int longitudeIndex = 0;
	unsigned int longitudeHemisphereIndex = 0;
	unsigned int latitudeIndex = 0;
	unsigned int latitudeHemisphereIndex = 0;
	unsigned int altitudeIndex = 0;
	unsigned int numberOfSatellitesIndex = 0;
	unsigned int firmwareVersionIndex = 0;

	unsigned int index = 1;
	if (fields[0] == "BS") {
	  // BS,2370CF,A,160540,190311,4808.1291,N,01132.6191,E,525.0,06,27b
	    counterIndex = index++;
	    gpsStatusIndex =  index++;
	    timeIndex = index++;
	    dateIndex = index++;
	    latitudeIndex = index++;
	    latitudeHemisphereIndex = index++;
	    longitudeIndex = index++;
	    longitudeHemisphereIndex = index++;
	    altitudeIndex = index++;
	    numberOfSatellitesIndex = index++;
	    firmwareVersionIndex = index++;
	    valid_ = true;
	} else if (fields[0] == "BLSEC") {
	  if (fields.size() == 12) {
	    counterIndex = index++;
	    gpsStatusIndex =  index++;
	    timeIndex = index++;
	    dateIndex = index++;
	    latitudeIndex = index++;
	    latitudeHemisphereIndex = index++;
	    longitudeIndex = index++;
	    longitudeHemisphereIndex = index++;
	    altitudeIndex = index++;
	    index++;
	    numberOfSatellitesIndex = index++;
	    valid_ = true;
	  } else if (fields.size() == 9) {
	    timeIndex = index++;
	    dateIndex = index++;
	    gpsStatusIndex = index++;
	    latitudeIndex = index++;
	    latitudeHemisphereIndex = index++;
	    longitudeIndex = index++;
	    longitudeHemisphereIndex = index++;
	    counterIndex = index++;
	    valid_ = true;
	  }
	}

	if (valid_) {
	  //if (logger_.isDebugEnabled())
	  //  logger_.debugStream() << "valid fields";

	  // read counter value
	  counter_ = parseHex(fields[counterIndex]);

	  // set GPS reception status
	  status_ = fields[gpsStatusIndex];

	  // set GPS PPS time
	  dateTime_ = fields[dateIndex] + " " + fields[timeIndex];

	  // exctract actual location and altitude information
	  longitude_ = parseGpsCoord(fields[longitudeIndex], fields[longitudeHemisphereIndex]);
	  latitude_ = parseGpsCoord(fields[latitudeIndex], fields[latitudeHemisphereIndex]);

	  if (altitudeIndex > 0) {
	    altitude_ = parseFloat(fields[altitudeIndex]);
	  } else {
	    altitude_ = 0;
	  }

	  if (numberOfSatellitesIndex > 0) {
	    // add actual satellite count to ringbuffer
	    numberOfSatellites_ = parseInt(fields[numberOfSatellitesIndex]);
	  } else {
	    numberOfSatellites_ = 0;
	  }

	  if (firmwareVersionIndex > 0) {
	    firmwareVersion_ = fields[firmwareVersionIndex];
	  } else {
	    firmwareVersion_ = "";
	  }
	}

      }

      unsigned short Ticks::getNumberOfSatellites() const {
	return numberOfSatellites_;
      }

      float Ticks::getLongitude() const {
	return longitude_;
      }

      float Ticks::getLatitude() const {
	return latitude_;
      }

      short Ticks::getAltitude() const {
	return altitude_;
      }

      const char Ticks::getGpsStatus() const {
	return status_[0];
      }

      const std::string& Ticks::getDateTime() const {
	return dateTime_;
      }

      const std::string& Ticks::getFirmwareVersion() const {
	return firmwareVersion_;
      }

    }
  }
}
