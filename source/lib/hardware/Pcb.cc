#include <iostream>
#include <sstream>

#include "util/String.h"
#include "hardware/Pcb.h"
#include "data/MEvent.h"

#define VERSION                 "debT&nbsp;0.9.2"

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

      int Pcb::parseHex(const std::string& hexString) {
	int hexValue;

	std::istringstream iss(hexString);
	iss >> std::hex >> hexValue;

	return hexValue;
      }

      bool Pcb::isOpen() const {
	return comm_.isOpen();
      }

      data::Event::AP Pcb::read() {
	std::string line = comm_.receive();

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "read() input: '" << line << "'";

	if (line.length() > 0) {

	  fields_.clear();

	  util::String::split(line, fields_, ",");

	  if (fields_[0] == "BLSEC" || fields_[0] == "BS") {
	    parseGps(fields_);
	  } else if (fields_[0] == "BLSEQ" || fields_[0] == "BLSIG" || fields_[0] == "BLSTR") {
	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "read() parse fields for '" << fields_[0] << "'";
	    return parse(fields_);
	  } else {
	    logger_.warnStream() << "unknown data " << fields_[0];
	  }
	}

	if (logger_.isDebugEnabled())
	  logger_.debug("read() returning empty event");

	return data::Event::AP();
      }

      void Pcb::parseGps(const std::vector<std::string>& fields) {
	if (fields.size() == 9) {
	  std::vector<std::string> reorderedFields;

	  reorderedFields.push_back(fields[0]);
	  reorderedFields.push_back(fields[8]);
	  reorderedFields.push_back(fields[3]);
	  reorderedFields.push_back(fields[1]);
	  reorderedFields.push_back(fields[2]);
	  reorderedFields.push_back(fields[4]);
	  reorderedFields.push_back(fields[5]);
	  reorderedFields.push_back(fields[6]);
	  reorderedFields.push_back(fields[7]);
	  reorderedFields.push_back("0");
	  reorderedFields.push_back("?");
	  reorderedFields.push_back("0");
	  gps_.parse(reorderedFields);
	} else {
	  gps_.parse(fields);
	}
      }

      data::Event::AP Pcb::parse(const std::vector<std::string> &fields) {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "parse() called";

	data::Event::AP event;
	data::Format::CP dataFormat;
	pt::time_duration sampleDt;
	std::string data = "";

	pt::ptime eventtime;

	// parse lighning event information
	if (fields[0] == "BLSIG") {
	  eventtime = gps_.getTime(parseHex(fields[1]));
	  logger_.infoStream() << "# of fields " << fields.size();
	  if (fields.size() > 3) {
	    logger_.infoStream() << "  fields[2] " << fields[2].size() << " '" << fields[2] << "' fields[3]# " << fields[3].size() << " '" << fields[3] << "'";
	    if (fields[2].size() == 2 && fields[3].size() ==2) {
	      dataFormat = data::Format::CP(new data::Format(8, 2, 1));
	      sampleDt = pt::nanoseconds(0);
	      data = fields[2] + fields[3];
	    }
	    else if (fields[2].size() == 3 && fields[3].size() ==3) {
	      dataFormat = data::Format::CP(new data::Format(12, 2, 1));
	      sampleDt = pt::nanoseconds(0);
	      data = fields[2] + fields[3];
	    }
	  } else {
	    if (fields[2].size() == 256) {
	      dataFormat = data::Format::CP(new data::Format(8, 2, 64));
	      sampleDt = pt::nanoseconds(3125);
	      data = fields[2];
	    }
	  }
	} else if (fields[0] == "BLSTR") {
	  eventtime = gps_.getTime(parseHex(fields[1]));

	} else if (fields[0] == "BLSEQ") {
	  eventtime = gps_.getTime(parseHex(fields[1]));
	    if (fields[2].size() == 256) {
	      dataFormat = data::Format::CP(new data::Format(8, 2, 64));
	      sampleDt = pt::nanoseconds(3125);
	      data = fields[2];
	    }

	} else {
	  logger_.errorStream() << "parse() data header '" << fields[0] << "' mismatch";
	}
	
	if (gps_.isValid() && eventtime != pt::not_a_date_time) {
	    if (logger_.isInfoEnabled())
	      logger_.infoStream() << "ѕtring data size: " << data.size();

	    if (logger_.isDebugEnabled())
	      logger_.debugStream() << "parse() event ready";

	  } else {
	    logger_.warnStream() << "GPS information is not yet valid -> no event created";
	  }

	int numberOfEvents = data.size() >> 2;

	if (logger_.isInfoEnabled())
	  logger_.infoStream() << "parseData() " << numberOfEvents << " to format " << *dataFormat;
    
	data::Array::AP array(new data::Array(dataFormat));

	int offset = 1 << (dataFormat->getNumberOfBitsPerSample() - 1);

	logger_.infoStream() << "offset: " << offset;

	for (int i=0; i < numberOfEvents; i++) {

	  int index = i << 2;

	  short xval = parseHex(data.substr(index, 2)) - offset;
	  short yval = parseHex(data.substr(index + 2, 2)) - offset;

	  array->set(xval, i, 0);
	  array->set(yval, i, 1);
	}

	data.append(" ");
	data.append(VERSION);
	data.append(" ");
	data.append("-");
	data.append(" ");
	{
	  std::ostringstream oss;
	  oss << comm_.getBaudRate();
	  data.append(oss.str());
	}
	data.append(" ");
	data.append(gps_.getType());
	logger_.infoStream() << "gpsType '" << gps_.getType() << "'";

	logger_.infoStream() << "set data '" << data << "'";
	
	data::Waveform::AP wfm(new data::Waveform(array, eventtime, sampleDt));

	event = data::MEvent::AP(new data::MEvent(wfm, gps_.getInfo(), data));

	return event;
      }

  }
}

