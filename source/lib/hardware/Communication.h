#ifndef BLITZORTUNG_HARDWARE_COMMUNICATION_H_
#define BLITZORTUNG_HARDWARE_COMMUNICATION_H_

#include <string>

#include "hardware/SerialPort.h"

namespace blitzortung {
  namespace hardware {

    class Communication {

      private:
	// serial port object
	SerialPort serialPort_;

      public:

	//! Constructor
	Communication(SerialPort &);

	//! Destructor
	virtual ~Communication();

	void setBaudRate(const int);

	bool isOpen() const;

	std::string readLine();

    };

  }
}

#endif
