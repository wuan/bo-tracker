#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Base::Base(comm::Base& communication) :
	communication_(communication),
	satelliteCount_(data::Base::BUFFERSIZE),
	logger_("hardware.gps.Base")
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

      void Base::init(bool force) {

	const unsigned int targetBaudRate = communication_.getBaudRate();

	if (force) {
	  
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "init() force";

	  // fill vector of supported baud rates
	  std::vector<unsigned int> baudRates;
	  baudRates.push_back(4800);
	  baudRates.push_back(19200);

	  // send initialization for every supported baud rate except the target baud rate
	  for (std::vector<unsigned int>::const_iterator baudRate=baudRates.begin(); baudRate != baudRates.end(); baudRate++) {
	    if (*baudRate != targetBaudRate) {

	      if (logger_.isDebugEnabled())
		logger_.debugStream() << "init() @ " << *baudRate << " set to " << targetBaudRate << " baud";
		
	      // switch to baud rate
	      communication_.setBaudRate(*baudRate);
	      
	      sleep(1);

	      // send initialization
	      initWrite(targetBaudRate);
	      
	      sleep(1);
	    }
	  }
	}

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "init() @ " << targetBaudRate << " set to " << targetBaudRate << " baud";
	  
	// switch the interface to the target baud rate
	communication_.setBaudRate(targetBaudRate);
	
	sleep(1);
	
	// send initalization with the target baud rate at the end
	initWrite(targetBaudRate);

	sleep(1);

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

	  // exctract actual location and altitude information
	  location_.addLongitude(fields[7], fields[8][0]);
	  location_.addLatitude(fields[5], fields[6][0]);
	  location_.addAltitude(fields[9]);

	  // use of value in fields[10] ???
	  
	  // add actual satellite count to ringbuffer
	  addSatelliteCount(fields[11]);
	  
	} else {
	  throw exception::Base("blitzortung::hardware::gps::Base.parse() wrong data to parse");
	}
	if (logger_.isDebugEnabled())
	  logger_.debug("parse() done");
      }

      bo::data::GpsInfo::AP Base::getInfo() const {
	bo::data::GpsInfo::AP gpsInfo(new bo::data::GpsInfo(*this));

	return gpsInfo;
      }
    }
  }
}
