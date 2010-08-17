/*  

    Copyright (C) 2010       Andreas Würl <awuerl@gmx.net>

*/

#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "namespaces.h"
#include "data/Samples.h"
#include "exception/Base.h"
#include "Logger.h"

pt::time_duration parseTime(const std::string& inputString) {
  std::istringstream iss(inputString);

  pt::time_facet *facet = new pt::time_facet();

  if (inputString.size() <= 4)
    facet->format("%H%M");
  else
    facet->format("%H%M%S");
  
  iss.imbue(std::locale(std::locale::classic(), facet));

  pt::time_duration time;
  iss >> time;
  return time;
}

int main(int argc, char **argv) {

  std::string file = "";
  std::string startTimeString, endTimeString;

  bo::Logger logger("");

  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help", "show program help")
    ("input-file,i", po::value<std::string>(&file), "file name")
    ("starttime,s", po::value<std::string>(&startTimeString), "start time in HHMM or HHMMSS format")
    ("endtime,e", po::value<std::string>(&startTimeString), "end time in HHMM or HHMMSS format")
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

  if (! vm.count("input-file")) {
    std::cerr << "'input-file' missing\n";
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

  pt::time_duration startTime(pt::not_a_date_time);
  if (vm.count("starttime")) {
    startTime = parseTime(startTimeString);
  }
  std::cout << "starttime: " << startTime << std::endl;


  bo::data::Samples samples;

  samples.readFromFile(file);

  for (bo::data::Samples::CI sample = samples.begin(); sample != samples.end(); sample++) {
    std::cout << *sample << std::endl;
  }
      
}
