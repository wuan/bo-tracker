/*  

    Copyright (C) 2003-2010  Egon Wanke <blitzortung@gmx.org>
    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include <iostream>
#include <string>

#include "namespaces.h"
#include "hardware/pcb/V6.h"
#include "util/RingBuffer.h"
#include "exception/Base.h"




int main(int argc, char **argv) {

  std::string serialPort = "/dev/ttyUSB0";
  int serialBaudRate = 19200;
  bo::hardware::SerialPort serial(serialPort, serialBaudRate);

  bo::hardware::pcb::V6 driver(serial);

  boost::thread transferThread;

  while (driver.isOpen()) {

    std::auto_ptr<bo::data::sample::Base> sample = driver.read();

    if (sample.get() != 0) {
      boost::thread transferThread(boost::bind(transfer, sample));
    }


  }

}
