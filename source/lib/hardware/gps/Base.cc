#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Base.h"
#include "hardware/parsing/Ticks.h"

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

      void Base::addSatelliteCount(unsigned short satelliteCount) {
	satelliteCount_.add(satelliteCount);
      }

      unsigned short Base::getSatelliteCount() const {
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

      void Base::set(const hardware::parsing::Ticks& ticksParser) {

	// initialize gps modules at least once a day
	if (dateInitialized_ != pt::second_clock::universal_time().date()) {
	  init();
	}

	// set GPS reception status
	setStatus(ticksParser.getGpsStatus());

	// set GPS PPS time
	time_.setSecond(ticksParser.getDateTime(), ticksParser.getCounterValue());

	// exctract actual location and altitude information
	location_.addLongitude(ticksParser.getLongitude());
	location_.addLatitude(ticksParser.getLatitude());
	location_.addAltitude(ticksParser.getAltitude());

	// add actual satellite count to ringbuffer
	addSatelliteCount(ticksParser.getNumberOfSatellites());
      }

      bo::data::GpsInfo::AP Base::getInfo() const {
	bo::data::GpsInfo::AP gpsInfo(new bo::data::GpsInfo(*this));

	return gpsInfo;
      }
    }
  }
}
