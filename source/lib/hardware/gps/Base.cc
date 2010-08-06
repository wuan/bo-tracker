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
	satelliteCount_(data::Base::BUFFERSIZE)
      {
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

      void Base::initWrite(const unsigned int baudRate) {

	switch(type_) {
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
	    oss << "$PSRF100,1," << baudRate << ",8,1,0*38\r\n";
	    communication_.send(oss.str());
	    }
	    break;

	  case GARMIN:
	    throw exception::Base("hardware::gps::Base::initWrite() no init strings for Garmin devices yet!");

	  default:
	    throw exception::Base("hardware::gps::Base::initWrite() unhandled device type");
	}
      }

      void Base::init() {
	const unsigned int baudRate = communication_.getBaudRate();

	if (baudRate == 9600) {
	  initWrite(4800);
	  initWrite(9600);
	} else if (baudRate == 4800) {
	  initWrite(9600);
	  initWrite(4800);
	} else {
	  throw exception::Base("hardware::gps::Base::init() unhandled baud rate");
	}
      }


      void Base::parse(const std::vector<std::string> &fields) {
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
