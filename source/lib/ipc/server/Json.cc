#include <sys/socket.h>
#include <sys/un.h>

#include "data/Event.h"
#include "ipc/server/Json.h"

namespace blitzortung {
  namespace ipc {
    namespace server {

      Json::Json(const unsigned int socket, const Process& process, const hardware::Pcb& hardware) :
	Base(socket),
	process_(process),
	hardware_(hardware),
	logger_("ipc.server.Json")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialize for socket " << socket;
      }

      void Json::cmdGetInfo() {
	    json_object* jsonHardware = json_object_new_object();
	    json_object_object_add(jsonHardware, "firmware", json_object_new_string(hardware_.getFirmwareVersion().c_str()));
	    json_object_object_add(jsonResponse_, "hardware", jsonHardware);

	    const hardware::gps::Base& gps = hardware_.getGps();
	    json_object* jsonGps = json_object_new_object();
	    char gpsStatus = gps.getStatus();
	    json_object_object_add(jsonGps, "longitude", json_object_new_double(gps.getLocation().getLongitude()));
	    json_object_object_add(jsonGps, "longitudeError", json_object_new_double(gps.getLocation().getLongitudeError()));
	    json_object_object_add(jsonGps, "latitude", json_object_new_double(gps.getLocation().getLatitude()));
	    json_object_object_add(jsonGps, "latitudeError", json_object_new_double(gps.getLocation().getLatitudeError()));
	    json_object_object_add(jsonGps, "altitude", json_object_new_int(gps.getLocation().getAltitude()));
	    json_object_object_add(jsonGps, "altitudeError", json_object_new_int(gps.getLocation().getAltitudeError()));
	    pt::ptime gpsTime = gps.getTime();
	    std::ostringstream oss;
	    gr::date_facet *datefacet = new gr::date_facet();
	    datefacet->format("%Y-%m-%d");
	    oss.imbue(std::locale(std::locale::classic(), datefacet));
	    oss << gpsTime.date();
	    json_object_object_add(jsonGps, "date", json_object_new_string(oss.str().c_str()));
	    oss.str("");
	    oss << gpsTime.time_of_day();
	    json_object_object_add(jsonGps, "time", json_object_new_string(oss.str().c_str()));
	    json_object_object_add(jsonGps, "ticksPerSecond", json_object_new_double(gps.getTicksPerSecond()));
	    json_object_object_add(jsonGps, "tickError", json_object_new_double(gps.getTickError()));
	    json_object_object_add(jsonGps, "status", json_object_new_string_len(&gpsStatus, 1));
	    json_object_object_add(jsonGps, "satelliteCount", json_object_new_int(gps.getSatelliteCount()));
	    json_object_object_add(jsonGps, "type", json_object_new_string(gps.getType().c_str()));
	    json_object_object_add(jsonHardware, "gps", jsonGps);
	    
	    const hardware::comm::Base& comm = hardware_.getComm();
	    json_object* jsonComm = json_object_new_object();
	    json_object_object_add(jsonComm, "baudRate", json_object_new_int(comm.getBaudRate()));
	    json_object_object_add(jsonComm, "interfaceName", json_object_new_string(comm.getInterfaceName().c_str()));
	    json_object_object_add(jsonHardware, "comm", jsonComm);

	    json_object* jsonProcess = json_object_new_object();
	    json_object_object_add(jsonProcess, "numberOfSeconds", json_object_new_int(process_.getEventCountBuffer().getActualSize()));
	    json_object_object_add(jsonProcess, "numberOfEvents", json_object_new_int(process_.getEventCountBuffer().getSum()));
	    json_object_object_add(jsonProcess, "eventsPerSecond", json_object_new_double(process_.getEventCountBuffer().getAverage()));
	    json_object_object_add(jsonResponse_, "process", jsonProcess);
      }

      std::string Json::respond(const std::string& input) {
	jsonResponse_ = json_object_new_object();

	json_object* jsonObj = json_tokener_parse(input.c_str());
	if (jsonObj != 0) {
	  json_object* cmd_obj = json_object_object_get(jsonObj, "cmd");
	  if (cmd_obj != 0) {
	    std::string command(json_object_get_string(cmd_obj));

	    json_object_object_add(jsonResponse_, "command", json_object_new_string(command.c_str()));

	    if (command == "getInfo")
	      cmdGetInfo();
	    else if (command =="getActivity")
	      cmdGetActivity();
	    	    
	    json_object_put(cmd_obj);
	  }

	  json_object_put(jsonObj);
	} else {
	  std::string result = "could not parse command '" + input + "'";
	  json_object_object_add(jsonResponse_, "error", json_object_new_string(result.c_str()));
	  logger_.warnStream() << result;
	}

	std::string jsonString(json_object_to_json_string(jsonResponse_));
	json_object_put(jsonResponse_);

	return jsonString;
      }

      void Json::cmdGetActivity() {
	    json_object* jsonActivity = json_object_new_array();

	    const int totalNumberOfSeconds = process_.getEventCountBuffer().getActualSize();
	    const int totalNumberOfMinutes = totalNumberOfSeconds / 60;

	    logger_.noticeStream() << " seconds " << totalNumberOfSeconds << ", minutes " << totalNumberOfMinutes;

	    for (int minute = totalNumberOfMinutes; minute > 0; minute--) {
	      int eventsPerMinute = 0;
	      for (int second = 0; second < 60; second ++) {
		int index = (totalNumberOfMinutes - minute) * 60 + second;
		eventsPerMinute += process_.getEventCountBuffer()[index];
		if (second == 0 || second == 59)
		  logger_.noticeStream() << "     minute " << minute << " second " << second << " index " << index;
	      }
	      logger_.noticeStream() << "   sum per minute " << eventsPerMinute;
	      json_object_array_add(jsonActivity, json_object_new_int(eventsPerMinute));
	    }

	    json_object_object_add(jsonResponse_, "activity", jsonActivity);
      }


    }
  }
}
