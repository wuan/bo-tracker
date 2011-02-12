#include "hardware/parsing/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      float Base::parseFloat(const std::string& floatString) const {
	  float floatValue;
	  std::istringstream iss(floatString);
	  iss >> floatValue;

	  return floatValue;
      }

      unsigned int Base::parseHex(const std::string& hexString) const {
	  unsigned int value;
	  
	  std::istringstream iss(hexString);
	  iss >> std::hex >> value;
	  return value;
      }

      float Base::parseGpsCoord(const std::string &coordString, const char hemisphere) const {
	double degreeMinuteValue;

	std::istringstream iss(coordString);
	iss >> degreeMinuteValue;

	double decimalDegreeValue = floor(degreeMinuteValue / 100.0);

	decimalDegreeValue += (degreeMinuteValue - decimalDegreeValue * 100.0) / 60.0;

	if (hemisphere == 'W' || hemisphere == 'S')
	  decimalDegreeValue *= -1.0;

	return decimalDegreeValue;
      }

    }
  }
}
