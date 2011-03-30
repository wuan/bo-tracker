#include <sys/socket.h>
#include <sys/un.h>

#include <json/json.h>

#include "data/Event.h"
#include "ipc/server/Json.h"

namespace blitzortung {
  namespace ipc {
    namespace server {

      Json::Json(const unsigned int socket, const Process& process, const hardware::gps::Base& gps) :
	Base(socket),
	process_(process),
	gps_(gps),
	logger_("ipc.server.Json")
      {
	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initialize for socket " << socket;
      }

      std::string Json::respond(const std::string& input) {
	json_object* jsonResult = json_object_new_object();

	json_object* jsonObj = json_tokener_parse(input.c_str());
	if (int(jsonObj) > 0) {
	  json_object* cmd_obj = json_object_object_get(jsonObj, "cmd");
	  if (cmd_obj != 0) {
	    const char* command = json_object_get_string(cmd_obj);
	    json_object_object_add(jsonResult, "command", json_object_new_string(command));

	    json_object* jsonGps = json_object_new_object();
	    char gpsStatus = gps_.getStatus();
	    json_object_object_add(jsonGps, "longitude", json_object_new_double(gps_.getLocation().getLongitude()));
	    json_object_object_add(jsonGps, "longitudeError", json_object_new_double(gps_.getLocation().getLongitudeError()));
	    json_object_object_add(jsonGps, "latitude", json_object_new_double(gps_.getLocation().getLatitude()));
	    json_object_object_add(jsonGps, "latitudeError", json_object_new_double(gps_.getLocation().getLatitudeError()));
	    json_object_object_add(jsonGps, "altitude", json_object_new_int(gps_.getLocation().getAltitude()));
	    json_object_object_add(jsonGps, "altitudeError", json_object_new_int(gps_.getLocation().getAltitudeError()));
	    pt::ptime gpsTime = gps_.getTime();
	    std::ostringstream oss;
	    gr::date_facet *datefacet = new gr::date_facet();
	    datefacet->format("%Y-%m-%d");
	    oss.imbue(std::locale(std::locale::classic(), datefacet));
	    oss << gpsTime.date();
	    json_object_object_add(jsonGps, "date", json_object_new_string(oss.str().c_str()));
	    oss.str("");
	    oss << gpsTime.time_of_day();
	    json_object_object_add(jsonGps, "time", json_object_new_string(oss.str().c_str()));
	    json_object_object_add(jsonGps, "tickError", json_object_new_double(gps_.getTickError()));
	    json_object_object_add(jsonGps, "status", json_object_new_string_len(&gpsStatus, 1));
	    json_object_object_add(jsonGps, "satelliteCount", json_object_new_int(gps_.getSatelliteCount()));
	    json_object_object_add(jsonResult, "gps", jsonGps);

	    json_object* jsonProcess = json_object_new_object();
	    json_object_object_add(jsonProcess, "numberOfSeconds", json_object_new_int(process_.getEventCountBuffer().getActualSize()));
	    json_object_object_add(jsonProcess, "numberOfEvents", json_object_new_int(process_.getEventCountBuffer().getSum()));
	    json_object_object_add(jsonProcess, "eventsPerSecond", json_object_new_double(process_.getEventCountBuffer().getAverage()));
	    json_object_object_add(jsonResult, "process", jsonProcess);

	    json_object_put(cmd_obj);
	  }

	  json_object_put(jsonObj);
	} else {
	  std::string result = "could not parse command '" + input + "'";
	  json_object_object_add(jsonResult, "error", json_object_new_string(result.c_str()));
	  logger_.warnStream() << result;
	}

	std::string jsonString(json_object_to_json_string(jsonResult));
	json_object_put(jsonResult);

	return jsonString;
      }

    }
  }
}
