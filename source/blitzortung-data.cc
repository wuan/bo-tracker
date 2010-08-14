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

int main(int argc, char **argv) {

  std::string file = "";

  bo::Logger logger("");

  // programm arguments/options
  boost::program_options::options_description desc("program options");
  desc.add_options()
    ("help", "show program help")
    ("input-file,i", po::value<std::string>(&file), "file name")
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

  bo::data::Samples samples;

  samples.readFromFile(file);

  for (bo::data::Samples::CI sample = samples.begin(); sample != samples.end(); sample++) {
    std::cout << *sample << std::endl;
  }
      
}
