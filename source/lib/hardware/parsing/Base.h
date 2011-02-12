#ifndef BLITZORTUNG_HARDWARE_PARSING_BASE_H_
#define BLITZORTUNG_HARDWARE_PARSING_BASE_H_

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include <iostream>

namespace blitzortung {
  namespace hardware {
    namespace parsing {

      //! base class for gps hardware objects      
      class Base {

	protected:

	  //! parse float string
	  float parseFloat(const std::string& floatString) const;

	  //! parse hexadecimal string
	  unsigned int parseHex(const std::string& hexString) const;

	  //! parse gps coordinate string
	  float parseGpsCoord(const std::string &coordString, const char hemisphere) const;

	public:

	  //! constructor
	  Base();

	  //! destructor
	  virtual ~Parse();
	  
	  virtual bool parse(const std::vector<std::string>) = 0;
      };

    }
  }
}
#endif
