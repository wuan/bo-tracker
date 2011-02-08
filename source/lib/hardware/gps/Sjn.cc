#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Sjn.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Sjn::Sjn(comm::Base& communication) :
	Base(communication),
	logger_("hardware.gps.Sjn")
      {
	init(true);
      }

      Sjn::~Sjn() {
      }

      void Sjn::initWrite(const unsigned int targetBaudRate) {

	if (targetBaudRate != 4800)
	  throw exception::Base("hardware::gps::Base::initWrite() unsupported target baud rate");

	communication_.send("PFEC,GPint,GGA01,GLL00,GSA00,GSV00,RMC01,DTM00,VTG00,ZDA00");

      }

      const std::string Sjn::getType() const {
	return "Sjn";
      }
    }
  }
}
