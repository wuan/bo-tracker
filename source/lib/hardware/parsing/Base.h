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

	  bool valid_;

	  unsigned int counter_;

	  //! parse int string
	  int parseInt(const std::string& floatString) const;

	  //! parse float string
	  float parseFloat(const std::string& floatString) const;

	  //! parse hexadecimal string
	  unsigned int parseHex(const std::string& hexString) const;

	  //! parse gps coordinate string
	  float parseGpsCoord(const std::string& coordString, const std::string& hemisphere) const;

	public:

	  //! constructor
	  Base();

	  //! destructor
	  virtual ~Base();
	  
	  bool isValid() const;
	  
	  unsigned int getCounterValue() const;
      };

    }
  }
}
#endif
