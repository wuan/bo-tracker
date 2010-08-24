#ifndef BLITZORTUNG_HARDWARE_COMM_BASE_H_
#define BLITZORTUNG_HARDWARE_COMM_BASE_H_

#include <string>

namespace blitzortung {
  namespace hardware {
    namespace comm {

      class Base {

	public:

	  //! Constructor
	  Base();

	  //! Destructor
	  virtual ~Base();

	  virtual void setBaudRate(const unsigned int) = 0;
	  virtual const unsigned int getBaudRate() const = 0;

	  virtual bool isOpen() const = 0;

	  //! receive data from the interface
	  virtual std::string receive() = 0;

	  //! send data to the interface
	  virtual void send(const std::string&) = 0;
      };

    }
  }
}

#endif
