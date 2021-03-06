#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Base.h"
#include "hardware/parsing/Ticks.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Base::Base(comm::Base& communication, const unsigned short baudRate, bool disableSbas) :
	communication_(communication),
	time_(),
	location_(),
	dateInitialized_(),
	satelliteCount_(data::Base::BUFFERSIZE),
	status_(0),
	logger_("hardware.gps.Base"),
	baudRate_(baudRate),
	disableSbas_(disableSbas)
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "Base(" << communication_.getBaudRate() << ", " << baudRate_ << ", " << disableSbas_ << ")";
      }

      Base::~Base() {
      }

      void Base::setStatus(const char status) {
	if (status != status_ && status_ != 0)
	  logger_.noticeStream() << "status change " << status_ << " -> " << status;
	status_ = status;
      }

      char Base::getStatus() const {
	return status_;
      }

      bool Base::isValid() const {
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

      pt::ptime Base::getTime() const {
	return time_.getTime();
      }

      pt::ptime Base::getTime(const int counter) const {
	return time_.getTime(counter);
      }

      double Base::getTicksPerSecond() const {
	return time_.getTicksPerSecond();
      }

      double Base::getTickError() const {
	return time_.getTickError();
      }

      const data::Location& Base::getLocation() const {
	return location_;
      }

      void Base::init(bool fullInitialization) {

	const unsigned int dataBaudRate = communication_.getBaudRate();

	if (fullInitialization) {

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "init() perform full initialization " << dataBaudRate << " baud for data ./. " << baudRate_ << " baud for GPS";

	  // fill vector of supported baud rates
	  std::vector<unsigned int> initializingBaudRates;

	  initializingBaudRates.push_back(4800);
	  initializingBaudRates.push_back(9600);
	  initializingBaudRates.push_back(19200);
	  initializingBaudRates.push_back(38400);

	  // send initialization for every supported baud rate except the target baud rate
	  for (auto baudRate=initializingBaudRates.begin(); baudRate != initializingBaudRates.end(); baudRate++) {
	    if (*baudRate != baudRate_) {

	      if (logger_.isDebugEnabled())
		logger_.debugStream() << "init() @ " << *baudRate << " baud to " << baudRate_ << " baud";

	      communication_.setBaudRate(*baudRate);

	      sleep(1);

	      initWrite(baudRate_);

	      sleep(1);
	    }
	  }
	}

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "init() @ " << baudRate_ << " baud";

	// switch the interface to the target baud rate
	communication_.setBaudRate(baudRate_);

	sleep(1);

	// send initalization with the target baud rate at the end
	initWrite(baudRate_);

	sleep(1);

	if (baudRate_ != dataBaudRate) {
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "init() restore data baud rate to " << dataBaudRate << " baud";
	  
	  // switch the interface to the target baud rate
	  communication_.setBaudRate(dataBaudRate);

	  sleep(1);
	}

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
	return bo::data::GpsInfo::AP(new bo::data::GpsInfo(*this));
      }

      unsigned short Base::getBaudRate() const {
	return baudRate_;
      }

      float Base::getLongitude() const {
	return location_.getLongitude();
      }

      float Base::getLatitude() const {
	return location_.getLatitude();
      }

      short Base::getAltitude() const {
	return location_.getAltitude();
      }
    }
  }
}
