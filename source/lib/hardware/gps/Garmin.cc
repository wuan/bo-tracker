#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Garmin.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Garmin::Garmin(comm::Base& communication, const unsigned short gpsBaudRate, bool disableSbas) :
	Base(communication, gpsBaudRate, disableSbas),
	logger_("hardware.gps.Garmin")
      {
	init(true);
      }

      Garmin::~Garmin() {
      }

      void Garmin::initWrite(const unsigned int targetBaudRate) {

	int targetBaudKey;
	switch(targetBaudRate) {
	  case 4800:
	    targetBaudKey = 3;
	    break;

	  case 9600:
	    targetBaudKey = 4;
	    break;

	  case 19200:
	    targetBaudKey = 5;
	    break;

	  default:
	    throw exception::Base("hardware.gps.Garmin() initWrite() unsupported BaudRate");
	}

	communication_.send("PGRMO,GPGGA,1");
	communication_.send("PGRMO,GPGSA,0");
	communication_.send("PGRMO,GPGSV,0");
	communication_.send("PGRMO,GPRMC,1");
	communication_.send("PGRMO,GPVTG,0");
	communication_.send("PGRMO,PGRMM,0");
	communication_.send("PGRMO,PGRMT,0");
	communication_.send("PGRMO,PGRME,0");
	communication_.send("PGRMO,PGRMB,0");
	communication_.send("PGRMCE");

	{
	  std::ostringstream oss;
	  oss << "PGRMC,,51.5,,,,,,,," << targetBaudKey << ",,2,4,";
	  communication_.send(oss.str());
	}
	
	if (disableSbas_)
	  communication_.send("PGRMC1,1,,,,,,,N,,,,,");
        else
	  communication_.send("PGRMC1,1,,,,,,,W,,,,,");
      }

      const std::string Garmin::getType() const {
	return "Garmin";
      }

    }
  }
}
