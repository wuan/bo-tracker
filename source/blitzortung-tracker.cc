/*  

    Copyright (C) 2003-2010  Egon Wanke <blitzortung@gmx.org>
    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "namespaces.h"
#include "hardware/comm/SerialPort.h"
#include "hardware/comm/Serial.h"
#include "hardware/gps/Garmin.h"
#include "hardware/gps/Sirf.h"
#include "hardware/gps/Sjn.h"
#include "hardware/Pcb.h"
#include "Process.h"
#include "network/transfer/Udp.h"
#include "output/File.h"
#include "output/None.h"
#include "util/RingBuffer.h"
#include "exception/Base.h"
#include "Logger.h"

//#include "ui/ui_blitzortung-tracker.h"

int main(int argc, char **argv) {

  std::string username, password, servername;
  unsigned short serverport;
  std::string serialPortName = "/dev/ttyUSB0";
  std::string outputFile = "";
  unsigned short serialBaudRate = 19200;
  unsigned short sleepTime = 20;
  double eventRateLimit = 1.0;
  std::string gpsType = "sirf";

  // create main logger
  bo::Logger logger("blitzortung-tracker");
  std::string logFileName = "";

  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help", "show program help")
    ("serial-device,d", po::value<std::string>(&serialPortName)->default_value(serialPortName), "path to serial device")
    ("baud-rate,b", po::value<unsigned short>(&serialBaudRate)->default_value(serialBaudRate), "baud rate of serial port (4800, 9600, 19200, 38400)")
    ("username,u", po::value<std::string>(&username), "username of blitzortung.org")
    ("password,p", po::value<std::string>(&password), "password of blitzortung.org")
    ("server-host,h", po::value<std::string>(&servername), "blitzortung.org servername")
    ("server-port", po::value<unsigned short>(&serverport)->default_value(8308), "blitzortung.org serverport")
    ("sleep-time,s", po::value<unsigned short>(&sleepTime)->default_value(sleepTime), "sleep time between data transmission")
    ("gps-type,g", po::value<std::string>(&gpsType)->default_value(gpsType), "type of gps device (sjn, garmin or sirf)")
    ("logfile", po::value<std::string>(&logFileName), "file name for log output (defaults to stdout)")
    ("event-rate-limit,l", po::value<double>(&eventRateLimit)->default_value(eventRateLimit), "limit of event rate (in events per second) 1.0 means max. 3600 events per hour")
    ("output,o", po::value<std::string>(&outputFile), "output file name (e.g. Name_%Y%m%d.bor)")
    ("verbose,v", "verbose mode")
    ("debug", "debug mode")
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

  // logging setup

  if (vm.count("logfile")) {
    logger.setLogFile(logFileName);
  }

  logger.setPriority(log4cpp::Priority::NOTICE);

  if (vm.count("verbose")) {
    logger.setPriority(log4cpp::Priority::INFO);
  }

  if (vm.count("debug")) {
    logger.setPriority(log4cpp::Priority::DEBUG);
  }

  switch (serialBaudRate) {
    case 4800:
    case 9600:
    case 19200:
    case 38400:
      break;

    default:
      std::ostringstream oss;
      oss << "invalid serial baud rate: " << serialBaudRate;
      throw bo::exception::Base(oss.str());
  }

  // create serial port object

  bo::hardware::comm::SerialPort serialPort(serialPortName, serialBaudRate);
  bo::hardware::comm::Serial serial(serialPort);

  // select type of gps hardware

  bo::hardware::gps::Base::AP gps;

  if (gpsType == "garmin") {
    gps = bo::hardware::gps::Base::AP(new bo::hardware::gps::Garmin(serial));
  } else if (gpsType == "sirf") {
    gps = bo::hardware::gps::Base::AP(new bo::hardware::gps::Sirf(serial));
  } else if (gpsType == "sjn") {
    gps = bo::hardware::gps::Base::AP(new bo::hardware::gps::Sjn(serial));
  } else {
    std::ostringstream oss;
    oss << "invalid value of gps-type: '" << gpsType << "'";
    throw bo::exception::Base(oss.str());
  }

  // waveform format description
  bo::data::Format::AP wfmFormat;


  // create hardware driver object for blitzortung measurement hardware
  bo::hardware::Pcb hardware(serial, *gps);

  //! set credentials/parameters for network connection
  bo::network::Creds creds;
  creds.setServername(servername);
  creds.setServerport(serverport);
  creds.setUsername(username);
  creds.setPassword(password);

  bo::network::transfer::Base::AP transfer;

  transfer = bo::network::transfer::Base::AP(new bo::network::transfer::Udp(creds));

  bo::output::Base::AP output;

  if (vm.count("output")) {
    output = bo::output::Base::AP(new bo::output::File(outputFile));
  } else {
    output = bo::output::Base::AP(new bo::output::None());
  }

  //! create object of network driver for event transmission
  bo::Process process(*transfer, pt::seconds(sleepTime), eventRateLimit, *output);

  while (hardware.isOpen()) {

    bo::data::Event::AP event = hardware.read();

    if (event.get() != 0) {
      process.push(event);
    }

  }

}
