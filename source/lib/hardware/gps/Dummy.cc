#include "namespaces.h"
#include "exception/Base.h"
#include "hardware/gps/data/Base.h"
#include "hardware/gps/Dummy.h"

namespace blitzortung {
  namespace hardware {
    namespace gps {

      Dummy::Dummy(comm::Base& communication) :
	Base(communication),
	logger_("hardware.gps.Dummy")
      {
      }

      Dummy::~Dummy() {
      }

      void Dummy::initWrite(const unsigned int targetBaudRate) {
      }

      const std::string Dummy::getType() const {
	return "Dummy";
      }
    }
  }
}
