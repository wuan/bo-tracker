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
	  SerialPort serialPort_;

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

	  void setBaudRate(const unsigned int);
	  const unsigned int getBaudRate() const;

	  bool isOpen() const;

	  std::string receive();

	  void send(const std::string&);
      };

    }
  }
}

#endif