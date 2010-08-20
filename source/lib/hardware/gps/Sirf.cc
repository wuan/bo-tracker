#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Sirf.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Sirf::Sirf(comm::Base& communication) :
	Base(communication),
	logger_("hardware.gps.Sirf")
      {
	init(true);
      }

      Sirf::~Sirf() {
      }

      void Sirf::initWrite(const unsigned int targetBaudRate) {

	// enable 1 HZ GCA message
	communication_.send("$PSRF103,00,00,01,01*25\r\n");

	// disable GLL message '
	communication_.send("$PSRF103,01,00,00,01*25\r\n");

	// disable GSA message '
	communication_.send("$PSRF103,02,00,00,01*26\r\n");

	// disable GSV message '
	communication_.send("$PSRF103,03,00,00,01*27\r\n");

	// enable 1 HZ RMC message '
	communication_.send("$PSRF103,04,00,01,01*21\r\n");

	// disable VTG message '
	communication_.send("$PSRF103,05,00,00,01*21\r\n");
	//communication_.send("$PSRF103,06,00,00,01*22\r\n");
	//communication_.send("$PSRF103,08,00,00,01*2C\r\n");

	// enable NMEA protocol, baud rate, 8 data bits, 1 stop bit, no parity '
	{
	  std::ostringstream oss;
	  oss << "$PSRF100,1," << targetBaudRate << ",8,1,0*38\r\n";
	  communication_.send(oss.str());
	}
      }

    }
  }
}
