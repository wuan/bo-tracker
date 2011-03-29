#ifndef BLITZORTUNG_HARDWARE_COMM_BASE_H_
#define BLITZORTUNG_HARDWARE_COMM_BASE_H_

#include <string>

namespace blitzortung {
  namespace hardware {
    namespace comm {

      //! base class for communication interfaces
      class Base {

	public:

	  typedef std::auto_ptr<Base> AP;

	public:

	  //! Constructor
	  Base();

	  //! Destructor
	  virtual ~Base();

	  //! set baud rate
	  virtual void setBaudRate(const unsigned int) = 0;
	  
	  //! returns the actual baud rate
	  virtual const unsigned int getBaudRate() const = 0;

	  //! returns true if the communication is active
	  virtual bool isOpen() const = 0;

	  //! returns valid lines received at the interface
	  virtual std::string receive() = 0;

	  //! send data via the interface
	  virtual void send(const std::string&) = 0;
      };

    }
  }
}

#endif
