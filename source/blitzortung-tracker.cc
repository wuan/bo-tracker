/*  

    Copyright (C) 2003-2010  Egon Wanke <blitzortung@gmx.org>
    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include <iostream>
#include <string>

#include "namespaces.h"
#include "hardware/pcb/V6.h"
#include "network/Base.h"
#include "util/RingBuffer.h"
#include "exception/Base.h"

int main(int argc, char **argv) {

  std::string serialPort = "/dev/ttyUSB0";
  int serialBaudRate = 19200;

  bo::hardware::SerialPort serial(serialPort, serialBaudRate);

  //! hardware driver for blitzortung measurement hardware
  std::auto_ptr<bo::hardware::pcb::Base> hardware(new bo::hardware::pcb::V6(serial));

  //! network driver for sample transmission
  std::auto_ptr<bo::network::Base> network(new bo::network::Base());


  while (hardware->isOpen()) {

    std::auto_ptr<bo::data::sample::Base> sample = hardware->read();

    if (sample.get() != 0) {
      network->put(sample);
    }

  }

}
