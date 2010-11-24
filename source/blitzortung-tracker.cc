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
#include "hardware/pcb/V4.h"
#include "hardware/pcb/V6.h"
#include "data/sample/V1.h"
#include "data/sample/V2.h"
#include "network/Base.h"
#include "output/File.h"
#include "output/None.h"
#include "util/RingBuffer.h"
#include "exception/Base.h"
#include "Logger.h"

//#include "ui/ui_blitzortung-tracker.h"

int main(int argc, char **argv) {

//  QMainWindow mainWindow;
  std::string username, password, servername;
  unsigned short serverport;
  std::string serialPortName = "/dev/ttyUSB0";
  std::string outputFile = "";
  unsigned short serialBaudRate = 19200;
  unsigned short sleepTime = 20;
  unsigned short sampleVersion = 2;
  unsigned char pcbVersion = 6;
  double eventRateLimit = 1.0;
  std::string gpsType = "sirf";

  bo::Logger logger("");

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
    ("pcb-version", po::value<unsigned char>(&pcbVersion)->default_value(pcbVersion), "version of PCB (4 or 6)")
    ("event-rate-limit,l", po::value<double>(&eventRateLimit)->default_value(eventRateLimit), "limit of event rate (in events per second) 1.0 means max. 3600 events per hour")
    ("output,o", po::value<std::string>(&outputFile), "output file name (e.g. Name_%Y%m%d.bor)")
    ("output-format", po::value<unsigned short>(&sampleVersion)->default_value(sampleVersion), "output file format version")
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

  // create sample creator object
  bo::data::sample::Base::Creator::AP sampleCreator;

  switch (sampleVersion) {
    case 1:
      sampleCreator = bo::data::sample::Base::Creator::AP(new bo::data::sample::V1::Creator());
      break;

    case 2:
      sampleCreator = bo::data::sample::Base::Creator::AP(new bo::data::sample::V2::Creator());
      break;

    default:
      std::ostringstream oss;
      oss << "invalid sample version: " << pcbVersion;
      throw bo::exception::Base(oss.str());
  }

  // create hardware driver object for blitzortung measurement hardware
  bo::hardware::pcb::Base::AP hardware;

  switch (pcbVersion) {
    case 4:
      hardware = bo::hardware::pcb::Base::AP(new bo::hardware::pcb::V4(serial, *gps));
      break;

    case 6:
      hardware = bo::hardware::pcb::Base::AP(new bo::hardware::pcb::V6(serial, *gps));
      break;

    default:
      std::ostringstream oss;
      oss << "invalid pcb version: " << pcbVersion;
      throw bo::exception::Base(oss.str());
  }

  //! set credentials/parameters for network connection
  bo::network::Creds creds;
  creds.setServername(servername);
  creds.setServerport(serverport);
  creds.setUsername(username);
  creds.setPassword(password);

  bo::output::Base::AP output;

  if (outputFile != "") {
    output = bo::output::Base::AP(new bo::output::File(outputFile, *sampleCreator));
  } else {
    output = bo::output::Base::AP(new bo::output::None());
  }

  //! create object of network driver for sample transmission
  bo::network::Base::AP network(new bo::network::Base(creds, sleepTime, eventRateLimit, *output));

  while (hardware->isOpen()) {

    bo::data::Sample::AP sample = hardware->read();

    if (sample.get() != 0) {
      network->push(sample);
    }

  }

}
