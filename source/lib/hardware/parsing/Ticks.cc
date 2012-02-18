#include "hardware/parsing/Ticks.h"
#include "exception/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      Ticks::Ticks(const std::vector<std::string>& fields) :
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
	if (fields[0] == "BS" || fields[0] == "S") {
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
	  // read counter value
	  counter_ = parseHex(fields[counterIndex]);

	  // set GPS reception status
	  status_ = fields[gpsStatusIndex];

	  // set GPS PPS time
	  pt::time_input_facet *facet = new pt::time_input_facet();
	  facet->format("%d%m%y %H%M%S");
	  std::string dateTime = fields[dateIndex] + " " + fields[timeIndex];
	  std::istringstream dateTimeInput(dateTime);
	  dateTimeInput.imbue(std::locale(std::locale::classic(), facet));
	  dateTimeInput >> dateTime_;

	  if (dateTime_.is_not_a_date_time())
	    logger_.warnStream() << "Ticks() could not parse datetime '" << dateTime << "'";

	  // exctract actual location and altitude information
	  longitude_ = parseGpsCoord(fields[longitudeIndex], fields[longitudeHemisphereIndex]);
	  latitude_ = parseGpsCoord(fields[latitudeIndex], fields[latitudeHemisphereIndex]);

	  if (altitudeIndex > 0) {
	    altitude_ = round(parseFloat(fields[altitudeIndex]));
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

      const float Ticks::getLongitude() const {
	return longitude_;
      }

      const float Ticks::getLatitude() const {
	return latitude_;
      }

      short Ticks::getAltitude() const {
	return altitude_;
      }

      const char Ticks::getGpsStatus() const {
	return status_[0];
      }

      const pt::ptime& Ticks::getDateTime() const {
	return dateTime_;
      }

      const std::string& Ticks::getFirmwareVersion() const {
	return firmwareVersion_;
      }

    }
  }
}
