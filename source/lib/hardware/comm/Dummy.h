#ifndef BLITZORTUNG_HARDWARE_COMM_SERIAL_H_
#define BLITZORTUNG_HARDWARE_COMM_SERIAL_H_

#include <string>

#include "Logger.h"
#include "hardware/comm/Base.h"

namespace blitzortung {
  namespace hardware {
    namespace comm {

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

	  void setBaudRate(const unsigned int);
	  const unsigned int getBaudRate() const;

	  bool isOpen() const;

	  std::string receive();

	  void send(const std::string&);

	  void addReceivedLine(const std::string&);
      };

    }
  }
}

#endif
