#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/Pcb.h"
#include "data/MEvent.h"
#include "hardware/parsing/Ticks.h"

#define TRACKER_VERSION "debT&nbsp;0.9.5"

namespace blitzortung {
  namespace hardware {

    Pcb::Pcb(comm::Base& comm, gps::Base& gps, const std::string& firmwareVersion) :
      comm_(comm),
      gps_(gps),
      firmwareVersion_(firmwareVersion),
      logger_("hardware.Pcb")
    {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "initialized";
    }

    Pcb::~Pcb() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "destroyed";
    }

    bool Pcb::isOpen() const {
      return comm_.isOpen();
    }
    

    const std::string& Pcb::getFirmwareVersion() const {
      return firmwareVersion_;
    }
	
    const gps::Base& Pcb::getGps() const {
      return gps_;
    }
	
    const comm::Base& Pcb::getComm() const {
      return comm_;
    }

    data::Event::AP Pcb::read() {
      std::string line = comm_.receive();

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "read() input: '" << line << "'";

      if (line.length() > 0) {

	std::vector<std::string> fields;

	util::String::split(line, fields, ",");

	parsing::Ticks ticksParser(fields);

	if (ticksParser.isValid()) {
	  //if (logger_.isDebugEnabled())
	  //  logger_.debugStream() << "read() TicksParser is valid";

	  gps_.set(ticksParser);

	  if (ticksParser.getFirmwareVersion() != "")
	    firmwareVersion_ = ticksParser.getFirmwareVersion();

	  if (lastSampleCreated_.is_not_a_date_time()) {
	    lastSampleCreated_ = gps_.getTime();
	  }

	  if (gps_.getTime() - lastSampleCreated_ >= pt::minutes(10)) {
	    lastSampleCreated_ = gps_.getTime();
	    return createKeepaliveSample();
	  }

	} else {
	  parsing::Samples samplesParser(fields, gps_);

	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "read() parse samples";

	  if (samplesParser.isValid()) { 
	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "read() set lastSampleCreated to " << gps_.getTime();
	    lastSampleCreated_ = gps_.getTime();
	    return createSample(samplesParser);
	  } else {
	    logger_.warnStream() << "unknown data '" << line << "'";
	  }
	}
      }

      return data::Event::AP();
    }

    data::Event::AP Pcb::createKeepaliveSample() {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "createKeepaliveSample()";

      data::Waveform::AP waveform(new data::Waveform(gps_.getTime()));

      std::string rawData("-");
      rawData.append(getInfoString());

      data::Event::AP event(new data::MEvent(waveform, gps_.getInfo(), rawData));

      return event;
    }

    data::Event::AP Pcb::createSample(parsing::Samples& samplesParser) {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "createSample()";

      data::Waveform::AP waveform = samplesParser.getWaveform();

      std::string rawData = samplesParser.getRawData();

      if (gps_.isValid() && waveform->getTime() != pt::not_a_date_time) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "createSample() waveform " << *waveform;

	rawData.append(getInfoString());

	return std::move(data::Event::AP(new data::MEvent(waveform, gps_.getInfo(), rawData)));
      } else {
	logger_.warnStream() << "createSample() GPS information is not yet valid -> no event created";
      }

      return std::move(data::Event::AP());
    }

    std::string Pcb::getInfoString() const {
	std::string rawData;

	rawData.append(" ");

	rawData.append(TRACKER_VERSION);
	rawData.append(" ");

	if (firmwareVersion_ != "") {
	  rawData.append(firmwareVersion_);
	} else {
	  rawData.append("-");
	}
	rawData.append(" ");

	{
	  std::ostringstream oss;
	  oss << comm_.getBaudRate();
	  rawData.append(oss.str());
	}
	rawData.append(" ");

	rawData.append(gps_.getType());
	return rawData;
    }

  }
}

