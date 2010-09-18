#ifndef BLITZORTUNG_HARDWARE_COMM_SERIAL_H_
#define BLITZORTUNG_HARDWARE_COMM_SERIAL_H_

#include <string>

#include "Logger.h"
#include "hardware/comm/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      //! class for dummy communication
      /*!
      this class helps to debug and test the device communication without the need for a connected device
      
      the required communication to be received from the device can be set with a method of this object before its use.      
      */
      class Dummy : public Base {

	private:
	
	  //! array for storing dummy data
	  std::vector<std::string> dummyData_;

	  //! actual read position in dummy data
	  unsigned int readPosition_;

	  //! dummy baud rate
	  unsigned int dummyBaud_;

	  //! logger for classA
	  mutable Logger logger_;

	public:

	  //! Constructor
	  Dummy();

	  //! Destructor
	  virtual ~Dummy();

	  //! set baud rate
	  void setBaudRate(const unsigned int);
	  
	  //! returns the actual baud rate
	  const unsigned int getBaudRate() const;

	  //! returns true if the communication is active
	  bool isOpen() const;

	  //! returns valid lines received at the interface
	  std::string receive();

	  //! send string via the interface
	  void send(const std::string&);

	  //! add line to be "received" later (dummy function)
	  void addReceivedLine(const std::string&);
      };

    }
  }
}

#endif
