#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/Pcb.h"
#include "data/MEvent.h"
#include "data/WaveformOf.h"
#include "hardware/parsing/Ticks.h"


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

    const std::string Pcb::version_ = "debT&nbsp;1.0.1";

    bool Pcb::isOpen() const {
      return comm_.isOpen();
    }
    
    const std::string& Pcb::getFirmwareVersion() const {
      return firmwareVersion_;
    }

    const std::string& Pcb::getVersion() const {
      return version_;
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
	  gps_.set(ticksParser);

	  if (ticksParser.getFirmwareVersion() != "")
	    firmwareVersion_ = ticksParser.getFirmwareVersion();

	  if (lastSampleCreated_.is_not_a_date_time()) {
	    lastSampleCreated_ = gps_.getTime();
	    return createKeepaliveSample();
	  } else {
	    if (gps_.getTime() - lastSampleCreated_ >= pt::minutes(10)) {
	      lastSampleCreated_ = gps_.getTime();
	      return createKeepaliveSample();
	    }
	  }

	} else {
	  parsing::Samples samplesParser(fields, gps_);

	  if (samplesParser.isValid()) { 
	    lastSampleCreated_ = gps_.getTime();

	    return createSample(samplesParser);
	  } else {
	    logger_.warnStream() << "handled data '" << line << "'";
	  }
	}
      }

      return data::Event::AP();
    }

    data::Event::AP Pcb::createKeepaliveSample() {
      if (logger_.isInfoEnabled())
	logger_.infoStream() << "createKeepaliveSample()";

      data::Waveform::AP waveform(new data::WaveformOf<signed char>(0, 0, gps_.getTime()));

      std::string rawData("-");
      appendInfoString(rawData);

      data::Event::AP event(new data::MEvent(std::move(waveform), gps_.getInfo(), rawData));

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

	appendInfoString(rawData);

	return data::Event::AP(new data::MEvent(std::move(waveform), gps_.getInfo(), rawData));
      } else {
	logger_.warnStream() << "createSample() GPS information is not yet valid -> no event created";
      }

      return data::Event::AP();
    }

    void Pcb::appendInfoString(std::string& rawData) const {
	rawData.append(" ");

	rawData.append(version_);
	rawData.append(" ");

	if (firmwareVersion_ != "") {
	  rawData.append(firmwareVersion_);
	} else {
	  rawData.append("-");
	}
	rawData.append(" ");

	{
	  std::ostringstream oss;
	  if (comm_.getBaudRate() != gps_.getBaudRate()) {
	    oss << gps_.getBaudRate() << "/";
	  }
	  oss << comm_.getBaudRate();
	  rawData.append(oss.str());
	}
	rawData.append(" ");

	rawData.append(gps_.getType());
    }

  }
}

