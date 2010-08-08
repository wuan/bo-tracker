/*  

    Copyright (C) 2003-2010  Egon Wanke <blitzortung@gmx.org>
    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "namespaces.h"
#include "hardware/pcb/V6.h"
#include "network/Base.h"
#include "util/RingBuffer.h"
#include "exception/Base.h"

int main(int argc, char **argv) {

  std::string username, password, servername;
  unsigned short serverport;
  std::string serialPort = "/dev/ttyUSB0";
  int serialBaudEnum = 2;
  int sleepTime = 20;
  double eventRateLimit = 1.0;
  std::string gpsType = "sirf";

  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help", "show program help")
    ("serial-device,d", po::value<std::string>(&serialPort)->default_value(serialPort), "path to serial device")
    ("baud-rate,b", po::value<int>(&serialBaudEnum)->default_value(serialBaudEnum), "baud rate of serial port (0: 4800, 2: 19200)")
    ("username,u", po::value<std::string>(&username), "username of blitzortung.org")
    ("password,p", po::value<std::string>(&password), "password of blitzortung.org")
    ("server-host,h", po::value<std::string>(&servername), "blitzortung.org servername")
    ("server-port", po::value<unsigned short>(&serverport)->default_value(8308), "blitzortung.org serverport")
    ("sleep-time,s", po::value<int>(&sleepTime)->default_value(sleepTime), "sleep time between data transmission")
    ("gps-type,g", po::value<std::string>(&gpsType)->default_value(gpsType), "type of gps device")
    ("event-rate-limit,l", po::value<double>(&eventRateLimit)->default_value(eventRateLimit), "limit of event rate (in events per second) 1.0 means max. 3600 events per hour")
    ("verbose,v", "verbose mode")
    ;

  // parse command line options
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm); 

  // help output or no 'sql-statement' given
  if (vm.count("help")) {
    std::cout << argv[0] << " [options]" << std::endl << std::endl;
    std::cout << desc << std::endl;
    std::cout << std::endl;

    return 1;
  }

  if (! vm.count("username")) {
    std::cerr << "'username' missing\n";
    return 5;
  }
      
  if (! vm.count("password")) {
    std::cerr << "'password' missing\n";
    return 5;
  }
      
  if (! vm.count("server-host")) {
    std::cerr << "'server-host' missing\n";
    return 5;
  }
      
  int serialBaudRate;

  switch (serialBaudEnum) {
    case 0:
      serialBaudRate = 4800;
      break;
    case 2:
      serialBaudRate = 19200;
      break;
    default:
      std::ostringstream oss;
      oss << "invalid enum for serialBaudRate: " << serialBaudEnum;
      throw bo::exception::Base(oss.str());
  }

  // select type of gps hardware

  bo::hardware::gps::Type gpsTypeEnum;

  if (gpsType == "garmin") {
    gpsTypeEnum = bo::hardware::gps::GARMIN;
  } else if (gpsType == "sirf") {
    gpsTypeEnum = bo::hardware::gps::SIRF;
  } else {
    std::ostringstream oss;
    oss << "invalid value of gps-type: '" << serialBaudEnum << "'";
    throw bo::exception::Base(oss.str());
  }

  // create serial port object

  bo::hardware::SerialPort serial(serialPort, serialBaudRate);

  // create hardware driver object for blitzortung measurement hardware
  std::auto_ptr<bo::hardware::pcb::Base> hardware(new bo::hardware::pcb::V6(serial, gpsTypeEnum));

  //! set credentials/parameters for network connection
  bo::network::Creds creds;
  creds.setServername(servername);
  creds.setServerport(serverport);
  creds.setUsername(username);
  creds.setPassword(password);

  //! create object of network driver for sample transmission
  std::auto_ptr<bo::network::Base> network(new bo::network::Base(creds, sleepTime, eventRateLimit));

  while (hardware->isOpen()) {

    std::auto_ptr<bo::data::sample::Base> sample = hardware->read();

    if (sample.get() != 0) {
      network->put(sample);
    }

  }

}
