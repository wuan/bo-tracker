#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Base::Base(const Communication &communication, const Type& type) :
	communication_(communication),
	type_(type),
	satelliteCount_(data::Base::BUFFERSIZE),
	logger_("hardware.gps.Base")
      {
	init(true);
      }

      Base::~Base() {
      }

      void Base::setStatus(const char status) {
	status_ = status;
      }

      const char Base::getStatus() const {
	return status_;
      }

      const bool Base::isValid() const {
	if (satelliteCount_.getActualSize() ==0)
	  return false;
	return true;
      }

      void Base::addSatelliteCount(const std::string &satelliteCountString) {
	int satelliteCount;
	std::istringstream iss(satelliteCountString);
	iss >> satelliteCount;
	satelliteCount_.add(satelliteCount);
      }

      int Base::getSatelliteCount() const {
	return satelliteCount_.getAverage();
      }

      pt::ptime Base::getTime(const int counter) const {
	return time_.getTime(counter);
      }

      const data::Location& Base::getLocation() const {
	return location_;
      }

      void Base::initWrite(const unsigned int baudRate, const unsigned int targetBaudRate) {

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initWrite() @ " << baudRate << " set to " << targetBaudRate << " baud";

	communication_.setBaudRate(baudRate);

	switch(type_) {
	  case SJN:
	    if (targetBaudRate != 4800)
	      throw exception::Base("hardware::gps::Base::initWrite() unsupported target baud rate");

	    communication_.send("$PFEC,GPint,GGA01,GLL00,GSA00,GSV00,RMC01,DTM00,VTG00,ZDA00*00\r\n");
	    break;

	  case SIRF:

	    // enable 1 HZ GCA message
	    communication_.send("$PSRF103,00,00,01,01*25\r\n");

	    // disable GLL message '
	    communication_.send("$PSRF103,01,00,00,01*25\r\n");

	    // disable GSA message '
	    communication_.send("$PSRF103,02,00,00,01*26\r\n");

	    // disable GSV message '
	    communication_.send("$PSRF103,03,00,00,01*27\r\n");

	    // enable 1 HZ RMC message '
	    communication_.send("$PSRF103,04,00,01,01*21\r\n");

	    // disable VTG message '
	    communication_.send("$PSRF103,05,00,00,01*21\r\n");
	    //communication_.send("$PSRF103,06,00,00,01*22\r\n");
	    //communication_.send("$PSRF103,08,00,00,01*2C\r\n");

	    // enable NMEA protocol, baud rate, 8 data bits, 1 stop bit, no parity '
	    {
	    std::ostringstream oss;
	    oss << "$PSRF100,1," << targetBaudRate << ",8,1,0*38\r\n";
	    communication_.send(oss.str());
	    }
	    break;

	  case GARMIN:

	    int targetBaudKey;
	    switch(targetBaudRate) {
	      case 4800:
		targetBaudKey = 3;
		break;

	      case 9600:
		targetBaudKey = 4;
		break;

	      case 19200:
		targetBaudKey = 5;
		break;

	      default:
		throw exception::Base("hardware::gps::Base::initWrite() unsupported target baud rate");
	    }

	    communication_.send("$PGRMO,GPGGA,1*00\r\n");
	    communication_.send("$PGRMO,GPGSA,0*00\r\n");
	    communication_.send("$PGRMO,GPGSV,0*00\r\n");
	    communication_.send("$PGRMO,GPRMC,1*00\r\n");
	    communication_.send("$PGRMO,GPVTG,0*00\r\n");
	    communication_.send("$PGRMO,PGRMM,0*00\r\n");
	    communication_.send("$PGRMO,PGRMT,0*00\r\n");
	    communication_.send("$PGRMO,PGRME,0*00\r\n");
	    communication_.send("$PGRMO,PGRMB,0*00\r\n");
	    communication_.send("$PGRMCE*00\r\n");
	    {
	      std::ostringstream oss;
	      oss << "$PGRMC,,51.5,,,,,,,," << targetBaudKey << ",,2,4,*00\r\n";
	      communication_.send(oss.str());
	    }
	    break;


	  default:
	    throw exception::Base("hardware::gps::Base::initWrite() unhandled device type");
	}
      }

      void Base::init(bool force) {

	const unsigned int targetBaudRate = communication_.getBaudRate();

	if (force) {
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "init() force";

	  // fill vector of supported baud rates
	  std::vector<unsigned int> baudRates;
	  baudRates.push_back(4800);
	  baudRates.push_back(19200);

	  for (std::vector<unsigned int>::const_iterator baudRate=baudRates.begin(); baudRate != baudRates.end(); baudRate++) {
	    if (*baudRate != targetBaudRate) {

	      initWrite(*baudRate, targetBaudRate);
	    }
	  }
	}

	initWrite(targetBaudRate, targetBaudRate);

	dateInitialized_ = pt::second_clock::universal_time().date();
      }

      void Base::parse(const std::vector<std::string> &fields) {

	// initialize gps modules at least once a day
	if (dateInitialized_ != pt::second_clock::universal_time().date()) {
	  init();
	}

	if (fields[0] == "BLSEC") {

	  // read counter value
	  int counter;
	  {
	    std::istringstream iss(fields[1]);
	    iss >> std::hex >> counter;
	  }

	  // set GPS reception status
	  setStatus(fields[2][0]);

	  // set GPS PPS time
	  time_.setSecond(fields[4] + " " + fields[3], counter);

	  //std::cout << time_.getSecond() << "\r" << std::flush;

	  location_.addLongitude(fields[7], fields[8][0]);
	  location_.addLatitude(fields[5], fields[6][0]);
	  location_.addAltitude(fields[9]);

	  // use of value in fields[10] ???
	  addSatelliteCount(fields[11]);
	  
	} else {
	  throw exception::Base("blitzortung::hardware::gps::Base.parse() wrong data to parse");
	}
      }
    }
  }
}
