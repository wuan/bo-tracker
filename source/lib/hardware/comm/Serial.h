#ifndef BLITZORTUNG_HARDWARE_COMM_SERIAL_H_
#define BLITZORTUNG_HARDWARE_COMM_SERIAL_H_

#include <string>

#include "hardware/comm/SerialPort.h"
#include "hardware/comm/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

      class Serial : public Base {

	private:
	  //! serial port object
	  SerialPort& serialPort_;

	  //! logger for this class
	  mutable Logger logger_;

	  //! calculation of checksum value
	  unsigned char calcChecksum(const std::string&);

	  //! check and return line data
	  std::string checkLine(const std::string&);

	public:

	  //! Constructor
	  Serial(SerialPort &);

	  //! Destructor
	  virtual ~Serial();

  	  //! set baud rate
	  void setBaudRate(const unsigned int);
	  
	  //! returns the actual baud rate
	  unsigned int getBaudRate() const;
	  
	  //! returns the interface name
	  const std::string& getInterfaceName() const;

	  //! returns true if the communication is active
	  bool isOpen() const;

	  //! returns valid lines received at the interface
	  std::string receive();

	  //! send string via the interface
	  void send(const std::string&);
      };

    }
  }
}

#endif
