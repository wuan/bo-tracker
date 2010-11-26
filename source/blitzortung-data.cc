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

pt::time_duration parseTime(const std::string& inputString, bool isEnd=false) {
  std::istringstream iss(inputString);
  pt::time_duration endOffset(pt::seconds(0));

  std::string format;
  if (inputString.size() <= 4) {
    format = "%H%M";
    endOffset = pt::minutes(1);
  } else {
    format = "%H%M%S";
    endOffset = pt::seconds(1);
  }

  // create formatting facet and set format for time_duration type
  pt::time_input_facet *facet = new pt::time_input_facet();
  facet->time_duration_format(format.c_str());
  
  iss.imbue(std::locale(std::locale::classic(), facet));

  pt::time_duration time;
  iss >> time;

  if (isEnd) 
    return time + endOffset;
  else
    return time;
}

void printSample(const bo::data::Sample& sample) {
    std::cout << sample << std::endl;
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
    ("endtime,e", po::value<std::string>(&endTimeString), "end time in HHMM or HHMMSS format")
    ("time-series,t", "output timeseries of single data points")
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

  pt::time_duration endTime(pt::not_a_date_time);
  if (vm.count("endtime")) {
    endTime = parseTime(endTimeString, true);
  }

  bo::data::Samples samples;

  samples.readFromFile(file, startTime, endTime);

  void (*sampleOperation)(const bo::data::Sample&);

  sampleOperation = &printSample;

  for (bo::data::Samples::CI sample = samples.begin(); sample != samples.end(); sample++) {
    (*sampleOperation)(*sample);
  }
      
}
