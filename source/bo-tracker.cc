/*  

    Copyright (C) 2003-2010  Egon Wanke <blitzortung@gmx.org>
    Copyright (C) 2010-2013  Andreas Würl <blitzortung@tryb.de>

*/

#include <iostream>
#include <exception>
#include <string>

#include <boost/program_options.hpp>

#include "namespaces.h"
#include "hardware/comm/SerialPort.h"
#include "hardware/comm/Serial.h"
#include "hardware/comm/Dummy.h"
#include "hardware/gps/Garmin.h"
#include "hardware/gps/Sirf.h"
#include "hardware/Pcb.h"
#include "Process.h"
#include "network/transfer/Udp.h"
#include "output/File.h"
#include "output/None.h"
#include "util/RingBuffer.h"
#include "exception/Base.h"
#include "ipc/UnixSocket.h"
#include "ipc/server/factory/Json.h"
#include "Logger.h"

int main(int argc, char **argv) {

  std::string username, password;
  std::string servername = "rechenserver.de";
  unsigned short serverport;
  std::string serialPortName = "/dev/ttyUSB0";
  std::string outputFile = "";
  unsigned int serialBaudRate = 19200;
  unsigned short gpsBaudRate = 4800;
  double eventRateLimit = 4.0;
  double amplitudeLimit = 0.05;
  std::string gpsType = "sirf";
  std::string firmwareVersion = "-";

  // create main logger
  bo::Logger logger;
  std::string logFileName = "";

  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help", "show program help")
    ("serial-device,d", po::value<std::string>(&serialPortName)->default_value(serialPortName), "path to serial device")
    ("baud-rate,b", po::value<unsigned int>(&serialBaudRate)->default_value(serialBaudRate), "baud rate of serial port (4800, 9600, 19200, 38400, 115200, 230400, 250000, 500000, 2500000)")
    ("username,u", po::value<std::string>(&username), "username of blitzortung.org")
    ("password,p", po::value<std::string>(&password), "password of blitzortung.org")
    ("server-host,h", po::value<std::string>(&servername)->default_value(servername), "blitzortung.org servername")
    ("server-port", po::value<unsigned short>(&serverport)->default_value(8308), "blitzortung.org serverport")
    ("gps-type,g", po::value<std::string>(&gpsType)->default_value(gpsType), "type of gps device (garmin or sirf)")
    ("gps-baud-rate", po::value<unsigned short>(&gpsBaudRate)->default_value(gpsBaudRate), "baud rate of gps serial port (4800, 9600, 19200, 38400) ignored for serial port baud rates < 115200")
    ("gps-disable-sbas", "disable GPS SBAS")
    ("logfile", po::value<std::string>(&logFileName), "file name for log output (defaults to stdout)")
    ("amplitude-limit,a", po::value<double>(&amplitudeLimit)->default_value(amplitudeLimit), "lower limit of signal amplitude")
    ("event-rate-limit,l", po::value<double>(&eventRateLimit)->default_value(eventRateLimit), "limit of event rate (in events per second) 1.0 means max. 3600 events per hour")
    ("output,o", po::value<std::string>(&outputFile), "output file name (e.g. Name_%Y%m%d.bor)")
    ("firmware-version", po::value<std::string>(&firmwareVersion)->default_value(firmwareVersion), "manually specify firmware version")
    ("verbose,v", "verbose mode")
    ("debug", "debug mode")
    ;

  po::variables_map vm;
  bool showHelp = false;

  try {
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm); 
  } catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    showHelp = true;
  }

  if (vm.count("help") || showHelp) {
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

  {
    std::string cmdline;
    for (int i=0; i<argc; i++) {
      cmdline.append(" ");
      cmdline.append(argv[i]);
    }
    logger.noticeStream() << "startup" << cmdline;
  }

  switch (serialBaudRate) {
    case 4800:
    case 9600:
    case 19200:
    case 38400:
      gpsBaudRate = serialBaudRate;
      break;

    case 115200:
    case 230400:
    case 250000:
    case 500000:
    case 2500000:
      switch (gpsBaudRate) {
	case 4800:
	case 9600:
	case 19200:
	case 38400:
	  break;

	default:
	  std::cerr << "invalid gps baud rate: " << serialBaudRate;
	  exit(10);
      }
      break;

    default:
      std::cerr << "invalid serial baud rate: " << serialBaudRate;
      exit(10);
  }

  bo::hardware::comm::Base::AP comm;
  bo::hardware::comm::SerialPort::AP serialPort;

  if (serialPortName == "dummy") {
    comm = bo::hardware::comm::Base::AP(new bo::hardware::comm::Dummy(true));
    comm->setBaudRate(serialBaudRate);
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,11C2CC,A,084638,200311,4648.1313,N,01332.6202,E,532.8,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,37E912,A,084639,200311,4648.1313,N,01332.6201,E,532.9,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,5E0F58,A,084640,200311,4648.1312,N,01332.6201,E,532.9,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,84359D,A,084641,200311,4648.1312,N,01332.6201,E,532.8,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,AA5BE3,A,084642,200311,4648.1312,N,01332.6201,E,532.8,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,D08229,A,084643,200311,4648.1313,N,01332.6202,E,532.8,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,F6A86F,A,084644,200311,4648.1314,N,01332.6203,E,532.7,09,27b");
    dynamic_cast<bo::hardware::comm::Dummy*>(comm.get())->addReceivedLine("BS,1CCEB4,A,084645,200311,4648.1314,N,01332.6203,E,532.7,09,27b");
  } else {
    serialPort = bo::hardware::comm::SerialPort::AP(new bo::hardware::comm::SerialPort(serialPortName, serialBaudRate));
    comm = bo::hardware::comm::Base::AP(new bo::hardware::comm::Serial(*serialPort));
  }


  bo::hardware::gps::Base::AP gps;
  {
    bool disableSbas = vm.count("gps-disable-sbas");

    if (gpsType == "garmin") {
      gps = bo::hardware::gps::Base::AP(new bo::hardware::gps::Garmin(*comm, gpsBaudRate, disableSbas));
    } else if (gpsType == "sirf") {
      gps = bo::hardware::gps::Base::AP(new bo::hardware::gps::Sirf(*comm, gpsBaudRate, disableSbas));
    } else {
      std::cerr << "invalid value of gps-type: '" << gpsType << "'";
      exit(10);
    }
  }

  bo::hardware::Pcb hardware(*comm, *gps, firmwareVersion);

  bo::network::Creds creds;
  creds.setServername(servername);
  creds.setServerport(serverport);
  creds.setUsername(username);
  creds.setPassword(password);

  bo::network::transfer::Udp transfer(creds);

  bo::output::Base::AP output;

  if (vm.count("output")) {
    output = bo::output::Base::AP(new bo::output::File(outputFile));
  } else {
    output = bo::output::Base::AP(new bo::output::None());
  }

  bo::Process process(transfer, eventRateLimit, *output);
  process.setAmplitudeLimit(amplitudeLimit);

  // setup unix domain socket for process information
  bo::ipc::server::factory::Json jsonServerFactory(process, hardware);
  bo::ipc::UnixSocket socket("/tmp/.blitzortung-tracker", jsonServerFactory);

  while (hardware.isOpen()) {

    auto event = hardware.read();

    if (event.get() != 0) {
      process.push(std::move(event));
    }

  }

}
