#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Sirf.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Sirf::Sirf(comm::Base& communication, const unsigned short gpsBaudRate, bool disableSbas) :
	Base(communication, gpsBaudRate, disableSbas),
	logger_("hardware.gps.Sirf")
      {
	init(true);
      }

      Sirf::~Sirf() {
      }

      void Sirf::initWrite(const unsigned int targetBaudRate) {

	if (logger_.isDebugEnabled())
	  logger_.debugStream() << "initWrite() " << targetBaudRate;

	// enable 1 HZ GCA message
	communication_.send("PSRF103,00,00,01,01");

	// disable GLL message '
	communication_.send("PSRF103,01,00,00,01");

	// disable GSA message '
	communication_.send("PSRF103,02,00,00,01");

	// disable GSV message '
	communication_.send("PSRF103,03,00,00,01");

	// enable 1 HZ RMC message '
	communication_.send("PSRF103,04,00,01,01");

	// disable VTG message '
	communication_.send("PSRF103,05,00,00,01");

	communication_.send("PSRF103,06,00,00,01");

	communication_.send("PSRF103,08,00,00,01");

	// enable NMEA protocol, baud rate, 8 data bits, 1 stop bit, no parity '
	{
	  std::ostringstream oss;
	  oss << "PSRF100,1," << targetBaudRate << ",8,1,0";
	  communication_.send(oss.str());
	}

	if (disableSbas_)
	  communication_.send("PSRF151,00");
        else
	  communication_.send("PSRF151,01");
      }

      const std::string Sirf::getType() const {
	return "SiRF";
      }
    }
  }
}
