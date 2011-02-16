#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/Pcb.h"
#include "data/MEvent.h"
#include "hardware/parsing/Ticks.h"

#define VERSION "debT&nbsp;0.9.2"

namespace blitzortung {
  namespace hardware {

    Pcb::Pcb(comm::Base& comm, gps::Base& gps) :
      comm_(comm),
      gps_(gps),
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

    data::Event::AP Pcb::read() {
      std::string line = comm_.receive();

      if (logger_.isInfoEnabled())
	logger_.infoStream() << "read() input: '" << line << "'";

      if (line.length() > 0) {

	std::vector<std::string> fields;

	util::String::split(line, fields, ",");

	parsing::Ticks ticksParser(fields);

	if (ticksParser.isValid()) {
	  if (logger_.isDebugEnabled())
	    logger_.debugStream() << "read() TicksParser is valid";

	  gps_.set(ticksParser);

	  if (ticksParser.getFirmwareVersion().size() > 0)
	    firmwareVersion_ = ticksParser.getFirmwareVersion();

	} else {
	  parsing::Samples samplesParser(fields, gps_);

	  if (samplesParser.isValid()) {

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "read() SampleParser is valid";

	    return createSample(samplesParser);
	  } else {
	    logger_.warnStream() << "unknown data '" << line << "'";
	  }
	}
      }

      if (logger_.isDebugEnabled())
	logger_.debug("read() returning empty event");

      return data::Event::AP();
    }

    data::Event::AP Pcb::createSample(parsing::Samples& samplesParser) {
      if (logger_.isDebugEnabled())
	logger_.debugStream() << "createSample() called";

      data::Waveform::AP waveform = samplesParser.getWaveform();

      std::string rawData = samplesParser.getRawData();

      if (gps_.isValid() && waveform->getTime() != pt::not_a_date_time) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "createSample() waveform " << *waveform;

	rawData.append(" ");

	rawData.append(VERSION);
	rawData.append(" ");

	if (firmwareVersion_.size() > 0)
	  rawData.append(firmwareVersion_);
	else 
	  rawData.append("-");
	rawData.append(" ");

	{
	  std::ostringstream oss;
	  oss << comm_.getBaudRate();
	  rawData.append(oss.str());
	}
	rawData.append(" ");

	rawData.append(gps_.getType());

	return data::Event::AP(new data::MEvent(waveform, gps_.getInfo(), rawData));
      } else {
	logger_.warnStream() << "createSample() GPS information is not yet valid -> no event created";
      }

      return data::Event::AP();
    }

  }
}

